; ================================================================
; CPE 342 Assignment 3a: Stepper Motor Controller - 8088 Assembly
; ================================================================
; Author: MSI Lab - Stepper Motor Control System
; Date: December 2025
; Purpose: Control stepper motor with buttons, timer interrupt, 
;          and 7-segment display using 8255 PPI and 8254 Timer
; ================================================================

; ======================== CONSTANTS =============================

; I/O Port Addresses
PPI_PORT_A      EQU 0x40        ; 8255 Port A (Stepper motor)
PPI_PORT_B      EQU 0x41        ; 8255 Port B (7-segment display)
PPI_PORT_C      EQU 0x42        ; 8255 Port C (Buttons/Status)
PPI_CONTROL     EQU 0x43        ; 8255 Control word register

TIMER_CNT0      EQU 0x60        ; 8254 Counter 0
TIMER_CONTROL   EQU 0x63        ; 8254 Control word register

; Button Flags (Port C bits)
BUTTON_A        EQU 0x01        ; PC0 - Clockwise
BUTTON_B        EQU 0x02        ; PC1 - Counter-clockwise
STATUS_LED      EQU 0x04        ; PC2 - Motor running indicator
TIMER_FLAG      EQU 0x08        ; PC3 - Timer interrupt flag

; Stepper Motor Sequence Values
STEP_0          EQU 0x01        ; 0001 - Coil A
STEP_1          EQU 0x02        ; 0010 - Coil B
STEP_2          EQU 0x04        ; 0100 - Coil C
STEP_3          EQU 0x08        ; 1000 - Coil D

; 7-Segment Display Codes (Common Cathode, Active High)
SEG_0           EQU 0x3F        ; 0
SEG_1           EQU 0x06        ; 1
SEG_2           EQU 0x5B        ; 2
SEG_3           EQU 0x4F        ; 3
SEG_4           EQU 0x66        ; 4
SEG_5           EQU 0x6D        ; 5
SEG_6           EQU 0x7D        ; 6
SEG_7           EQU 0x07        ; 7

; PPI Control Word
PPI_CTRL_WORD   EQU 0x89        ; PA=out, PB=out, PC_lower=in, PC_upper=out

; 8254 Timer Control Word
TIMER_CTRL_WD   EQU 0x36        ; Counter 0, Mode 2 (Rate Gen), 16-bit binary

; Timer Count Value for 1 second
; Clock: 1.193 MHz, For 1 sec: 1,193,182 cycles
TIMER_COUNT_L   EQU 0xE2        ; LSB
TIMER_COUNT_H   EQU 0x23        ; MSB (0x23E2 = 9186, but need 0x12345E for 1.2M)
; Actual: 1193182 = 0x12345E, we use programmed value

; Debounce timing
DEBOUNCE_DELAY  EQU 0x0FFF      ; ~20ms at 1MHz

; ====================== DATA SEGMENT ==========================

SEGMENT DATA_SEG

; Stepper motor sequence table (clockwise)
STEPPER_SEQUENCE:
    DB STEP_0                   ; Position 0: 0001
    DB STEP_1                   ; Position 1: 0010
    DB STEP_2                   ; Position 2: 0100
    DB STEP_3                   ; Position 3: 1000
    ; Wrapped sequence for 8 positions
    DB STEP_0                   ; Position 4 (repeat)
    DB STEP_1                   ; Position 5
    DB STEP_2                   ; Position 6
    DB STEP_3                   ; Position 7

; 7-Segment display lookup table
SEGMENT_TABLE:
    DB SEG_0                    ; Position 0 → Display "0"
    DB SEG_1                    ; Position 1 → Display "1"
    DB SEG_2                    ; Position 2 → Display "2"
    DB SEG_3                    ; Position 3 → Display "3"
    DB SEG_4                    ; Position 4 → Display "4"
    DB SEG_5                    ; Position 5 → Display "5"
    DB SEG_6                    ; Position 6 → Display "6"
    DB SEG_7                    ; Position 7 → Display "7"

; Motor position counter (0-7, wraps around)
MOTOR_POSITION: DB 0x00

; Previous button state (for debouncing)
PREV_BUTTON:    DB 0x00

; ISR context save area
SAVED_AX:       DW 0x0000
SAVED_DX:       DW 0x0000
SAVED_BP:       DW 0x0000

; Timer interrupt counter
INT_COUNT:      DW 0x0000

SEGMENT_END

; ===================== CODE SEGMENT ==========================

SEGMENT CODE_SEG

; ==================== INITIALIZATION ROUTINE ===================

INIT_SYSTEM PROC

    ; Save context
    PUSH AX
    PUSH DX
    
    ; ========== 1. Initialize 8255 PPI ==========
    MOV AL, PPI_CTRL_WORD       ; Control word: PA out, PB out, PC lower in
    MOV DX, PPI_CONTROL
    OUT DX, AL
    
    ; Set Port A to all zeros (stepper motor off)
    MOV AL, 0x00
    MOV DX, PPI_PORT_A
    OUT DX, AL
    
    ; Set Port B to initial display (Position 0 = "0")
    MOV AL, SEG_0
    MOV DX, PPI_PORT_B
    OUT DX, AL
    
    ; ========== 2. Initialize 8254 Timer ==========
    ; Control word: Counter 0, Mode 2 (Rate Generator), 16-bit binary
    MOV AL, TIMER_CTRL_WD
    MOV DX, TIMER_CONTROL
    OUT DX, AL
    
    ; Load timer count (LSB first, then MSB)
    ; For 1 second at 1.193 MHz: count = 1,193,182 (0x12345E)
    MOV AX, 0xE2A1             ; LSB of count value
    MOV DX, TIMER_CNT0
    OUT DX, AL                 ; Load LSB
    
    MOV AL, AH                 ; Load MSB
    OUT DX, AL
    
    ; ========== 3. Setup Interrupt Vectors ==========
    ; Set INT3 (Timer) vector to point to ISR_TIMER
    ; Interrupt vector table location: 0x0000C (INT 3)
    
    CLI                         ; Disable interrupts
    
    ; Set Interrupt Vector 3 (Timer)
    MOV AX, 0x0000
    MOV DS, AX
    MOV WORD PTR [0x0C], OFFSET ISR_TIMER  ; IP (offset)
    MOV WORD PTR [0x0E], CS                ; CS (segment)
    
    ; ========== 4. Clear Data Variables ==========
    MOV AL, 0x00
    MOV [MOTOR_POSITION], AL
    MOV [PREV_BUTTON], AL
    MOV [INT_COUNT], AX
    
    ; ========== 5. Enable Interrupts ==========
    STI                         ; Enable global interrupt flag
    
    ; Restore context
    POP DX
    POP AX
    
    RET
INIT_SYSTEM ENDP

; ==================== MAIN PROGRAM LOOP ====================

MAIN_LOOP PROC

main_start:
    ; Read button inputs from Port C
    MOV DX, PPI_PORT_C
    IN AL, DX
    AND AL, 0x03                ; Mask only buttons (PC0, PC1)
    
    ; Check if button state changed
    MOV BL, [PREV_BUTTON]
    CMP AL, BL
    JE main_loop_continue       ; No change, skip debounce
    
    ; Debounce: delay and re-read
    CALL DEBOUNCE_BUTTONS
    
    ; After debounce, AL contains stable button state
    MOV [PREV_BUTTON], AL
    
    ; Check which button is pressed
    TEST AL, BUTTON_A           ; Test PC0 (Button A)
    JNZ button_a_pressed
    
    TEST AL, BUTTON_B           ; Test PC1 (Button B)
    JNZ button_b_pressed
    
    ; No button pressed - jump to timer wait
    JMP main_loop_continue
    
button_a_pressed:
    ; Button A: Rotate Clockwise
    CALL ROTATE_CLOCKWISE
    JMP main_loop_continue
    
button_b_pressed:
    ; Button B: Rotate Counter-Clockwise
    CALL ROTATE_COUNTER_CLOCKWISE
    
main_loop_continue:
    ; Update 7-segment display with current position
    CALL UPDATE_SEGMENT_DISPLAY
    
    ; Small delay before next iteration
    MOV CX, 0x7FFF
    LOOP main_loop_continue
    
    JMP main_start              ; Infinite loop
    
MAIN_LOOP ENDP

; ==================== DEBOUNCE FUNCTION ======================

DEBOUNCE_BUTTONS PROC
    
    ; Input: AL = current button state
    ; Output: AL = debounced button state
    
    PUSH CX
    
    ; Delay ~20ms
    MOV CX, DEBOUNCE_DELAY
debounce_delay:
    NOP
    LOOP debounce_delay
    
    ; Re-read button state
    MOV DX, PPI_PORT_C
    IN AL, DX
    AND AL, 0x03                ; Mask buttons only
    
    POP CX
    RET
    
DEBOUNCE_BUTTONS ENDP

; ==================== ROTATE CLOCKWISE =======================

ROTATE_CLOCKWISE PROC
    
    PUSH AX
    PUSH BX
    PUSH DX
    
    ; Increment position counter
    MOV BL, [MOTOR_POSITION]
    INC BL
    
    ; Check wrap-around at 8 (0-7 range)
    CMP BL, 0x08
    JNE cw_no_wrap
    MOV BL, 0x00                ; Wrap to 0
    
cw_no_wrap:
    ; Save new position
    MOV [MOTOR_POSITION], BL
    
    ; Get stepper sequence value from table
    MOV AL, BL
    MOV BX, OFFSET STEPPER_SEQUENCE
    ADD BX, AX                  ; BX points to sequence value
    MOV AL, [BX]
    
    ; Output to stepper motor (Port A)
    MOV DX, PPI_PORT_A
    OUT DX, AL
    
    ; Toggle status LED (PC2)
    MOV DX, PPI_PORT_C
    IN AL, DX
    XOR AL, STATUS_LED          ; Toggle PC2
    OUT DX, AL
    
    POP DX
    POP BX
    POP AX
    RET
    
ROTATE_CLOCKWISE ENDP

; ================== ROTATE COUNTER-CLOCKWISE =================

ROTATE_COUNTER_CLOCKWISE PROC
    
    PUSH AX
    PUSH BX
    PUSH DX
    
    ; Decrement position counter
    MOV BL, [MOTOR_POSITION]
    DEC BL
    
    ; Check wrap-around below 0
    JNS ccw_no_wrap             ; If positive, no wrap
    MOV BL, 0x07                ; Wrap to 7
    
ccw_no_wrap:
    ; Save new position
    MOV [MOTOR_POSITION], BL
    
    ; Get stepper sequence value from table
    MOV AL, BL
    MOV BX, OFFSET STEPPER_SEQUENCE
    ADD BX, AX                  ; BX points to sequence value
    MOV AL, [BX]
    
    ; Output to stepper motor (Port A)
    MOV DX, PPI_PORT_A
    OUT DX, AL
    
    ; Toggle status LED (PC2)
    MOV DX, PPI_PORT_C
    IN AL, DX
    XOR AL, STATUS_LED          ; Toggle PC2
    OUT DX, AL
    
    POP DX
    POP BX
    POP AX
    RET
    
ROTATE_COUNTER_CLOCKWISE ENDP

; ================= UPDATE SEGMENT DISPLAY ====================

UPDATE_SEGMENT_DISPLAY PROC
    
    PUSH AX
    PUSH BX
    PUSH DX
    
    ; Get current motor position
    MOV BL, [MOTOR_POSITION]
    
    ; Mask to 0-7 range (safety)
    AND BL, 0x07
    
    ; Look up 7-segment code from table
    MOV AL, BL
    MOV BX, OFFSET SEGMENT_TABLE
    ADD BX, AX                  ; BX points to segment code
    MOV AL, [BX]
    
    ; Output to 7-segment display (Port B)
    MOV DX, PPI_PORT_B
    OUT DX, AL
    
    POP DX
    POP BX
    POP AX
    RET
    
UPDATE_SEGMENT_DISPLAY ENDP

; ================= INTERRUPT SERVICE ROUTINE ==================

ISR_TIMER PROC
    
    ; Save context
    PUSH AX
    PUSH DX
    
    ; Increment interrupt counter
    MOV AX, [INT_COUNT]
    INC AX
    MOV [INT_COUNT], AX
    
    ; Auto-rotate clockwise on timer interrupt
    CALL ROTATE_CLOCKWISE
    
    ; Update 7-segment display
    CALL UPDATE_SEGMENT_DISPLAY
    
    ; Send End-of-Interrupt (EOI) signal to 8088 Interrupt Controller
    MOV AL, 0x20                ; EOI command
    MOV DX, 0x20                ; Interrupt controller port
    OUT DX, AL
    
    ; Restore context
    POP DX
    POP AX
    
    IRET                        ; Return from interrupt
    
ISR_TIMER ENDP

; ===================== PROGRAM ENTRY POINT ====================

PROGRAM_START:
    
    ; Initialize data segment
    MOV AX, DATA_SEG
    MOV DS, AX
    
    ; Initialize stack pointer
    MOV AX, 0x2000              ; Stack segment
    MOV SS, AX
    MOV SP, 0xFFFF              ; Stack pointer (top of segment)
    
    ; Initialize code segment
    MOV AX, CODE_SEG
    MOV CS, AX
    
    ; Initialize all hardware
    CALL INIT_SYSTEM
    
    ; Enter main program loop
    CALL MAIN_LOOP
    
    ; Infinite loop (main loop never returns)
    JMP $
    
    HLT                         ; Halt (shouldn't reach here)

SEGMENT_END

; ======================= END OF PROGRAM =======================
; 
; Registers Usage:
;   AL/AH: General purpose, I/O data
;   BL/BH: General purpose, counters
;   CL/CH: Loop counters
;   DL/DH: I/O port addresses
;   
; Memory Usage:
;   DS:MOTOR_POSITION (1 byte): Current motor position (0-7)
;   DS:PREV_BUTTON (1 byte): Previous button state
;   DS:INT_COUNT (2 bytes): Timer interrupt counter
;   
; I/O Port Map:
;   0x40: 8255 Port A (Stepper motor)
;   0x41: 8255 Port B (7-segment display)
;   0x42: 8255 Port C (Buttons + LEDs)
;   0x43: 8255 Control word
;   0x60: 8254 Counter 0
;   0x63: 8254 Control word
;   0x20: 8088 Interrupt Controller (EOI)
;
; ===================================================================


.data
POS     DB 0           ; Motor position 0–3
STEP    DB 1,2,4,8     ; Stepper motor wave-drive sequence

; PPI macros
PPI_PORT_A      EQU 0x30        ; 8255 Port A (Stepper motor)
PPI_PORT_B      EQU 0x31        ; 8255 Port B (7-segment display)
PPI_PORT_C      EQU 0x32        ; 8255 Port C (Buttons/Status)
PPI_CWR         EQU 0x33        ; 8255 Control word register
; TIMER macros
TIMER_0         EQU 0x40        ; 8254 COUNTER 1 
TIMER_CWR       EQU 0x43        ; 8254 Control word register

.code 
; init PPI
MOV AL, 81h
out PPI_CWR , al
; init TIMER
MOV AL, 34H        ; 
OUT TIMER_CWR, AL

MOV AL, 40H        ; LSB of 1,000,000
OUT TIMER_0, AL
MOV AL, 42H        ; MSB of 1,000,000
OUT TIMER_0, AL