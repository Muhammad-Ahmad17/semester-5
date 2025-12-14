# CPE 342 Assignment 3a: Stepper Motor Controller with Position Display

## Project Overview
Design an 8088-based stepper motor control system with manual button control, automatic timer-based rotation, and 7-segment position display using 8255 PPI and 8254 Timer.

---

## 1. HARDWARE ARCHITECTURE

### 1.1 Component Integration

```
┌─────────────────────────────────────────────────────────────────┐
│                     8088 MICROPROCESSOR                         │
│                                                                 │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │  Control Unit | ALU | Registers (AX,BX,CX,DX,SI,DI)    │  │
│  │  Interrupt Controller | Address/Data Bus                │  │
│  └──────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
         │                    │                    │
    ─────┴──────────────  ────┴──────────────  ────┴──────────
    │                    │                    │
    ↓                    ↓                    ↓
┌─────────────┐   ┌──────────────┐   ┌─────────────────┐
│  8255 PPI   │   │  8254 Timer  │   │  Address Decoder│
│  (I/O Ctrl) │   │ (Interrupt)  │   │     (74138)     │
└─────────────┘   └──────────────┘   └─────────────────┘
    │ Port A         │ OUT0(Timer)
    │ Port B    │    │
    │ Port C    │    │
    ↓          ↓    ↓
┌──────────────────────────────────────────────┐
│  OUTPUT DEVICES                              │
│  ├─ Stepper Motor (4 coils: PA0-PA3)         │
│  ├─ 7-Segment Display (PB0-PB6, PB7=DP)     │
│  └─ Push Buttons (PC0=ButtonA, PC1=ButtonB) │
│     (Interrupt to INT0)                      │
└──────────────────────────────────────────────┘
```

### 1.2 Pin Configuration

**8255 PPI Pinout (8-bit Parallel Interface):**

```
Port A (PA7-PA0):
  PA3-PA0: Stepper Motor Coils (IN1, IN2, IN3, IN4)
  PA7-PA4: Reserved

Port B (PB7-PB0):
  PB6-PB0: 7-Segment Display Segments (a,b,c,d,e,f,g)
  PB7:     Decimal Point (DP)

Port C (PC7-PC0):
  PC0:     Button A (Clockwise) - Input
  PC1:     Button B (Counter-clockwise) - Input
  PC2:     Motor Running Status LED - Output
  PC3:     Timer Interrupt Flag - Output
  PC7-PC4: Reserved
```

**8254 Timer Configuration:**

```
Counter 0: 1-second interrupt timer
  - Clock Input: CLK (1.193 MHz)
  - Output: Connected to 8088 INT3
  - Mode: Rate Generator (Mode 2)
  - Count: 1,193,182 (for 1-second interval)
```

---

## 2. STEPPER MOTOR CONTROL

### 2.1 Motor Sequence (Full-Step Mode - 4-Step)

**Clockwise Rotation Sequence:**
```
Step 0: Coils = 0001 (1) → IN1 on
Step 1: Coils = 0010 (2) → IN2 on
Step 2: Coils = 0100 (4) → IN3 on
Step 3: Coils = 1000 (8) → IN4 on
(Repeat: 0→1→2→3→0...)

Position Display: 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7
(Cycles every 8 steps for modulo 8 position)
```

**Counter-Clockwise Rotation Sequence:**
```
Reverse of clockwise: 8→4→2→1→8...
Position Display: 0x0, 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1
```

### 2.2 Stepper Motor Coil Configuration

```
Half-Bridge Driver (ULN2003 or similar):

      8255 PA3-PA0              Motor Coils
         PA0 ─────────→ IN1 ─────→ Coil A
         PA1 ─────────→ IN2 ─────→ Coil B
         PA2 ─────────→ IN3 ─────→ Coil C
         PA3 ─────────→ IN4 ─────→ Coil D
```

---

## 3. 7-SEGMENT DISPLAY CONFIGURATION

### 3.1 Segment Mapping

```
        ┌─ a ─┐
        │     │
      f │     │ b
        ├─ g ─┤
      e │     │ c
        │     │
        └─ d ─┘  DP

8255 Port B Mapping:
PB0 = a (Top)
PB1 = b (Top-Right)
PB2 = c (Bottom-Right)
PB3 = d (Bottom)
PB4 = e (Bottom-Left)
PB5 = f (Top-Left)
PB6 = g (Middle)
PB7 = DP (Decimal Point)
```

### 3.2 Position Display Values (Common Cathode)

Position 0-7 → 7-Segment Hex Values (active-high):

```
Pos 0: 0x3F (0000)  → Display "0"   → Binary: 00111111
Pos 1: 0x06 (0001)  → Display "1"   → Binary: 00000110
Pos 2: 0x5B (0010)  → Display "2"   → Binary: 01011011
Pos 3: 0x4F (0011)  → Display "3"   → Binary: 01001111
Pos 4: 0x66 (0100)  → Display "4"   → Binary: 01100110
Pos 5: 0x6D (0101)  → Display "5"   → Binary: 01101101
Pos 6: 0x7D (0110)  → Display "6"   → Binary: 01111101
Pos 7: 0x07 (0111)  → Display "7"   → Binary: 00000111
```

---

## 4. 8255 PPI CONFIGURATION

### 4.1 Control Word (Mode Selection)

```
8255 Control Word: 1000 1001 (0x89)
                  │││││││└─ PA0: Output
                  ││││││└── PA1-PA3: Output
                  │││││└─── PA7-PA4: Output
                  ││││└──── PB: Output Mode
                  │││└───── PC Lower: Input (Buttons)
                  ││└────── PC Upper: Output
                  │└─────── Mode 0 (Simple I/O)
                  └──────── I/O Mode (1=I/O, 0=BSR)

Control Word = 0x89
- Port A: Output (for stepper motor)
- Port B: Output (for 7-segment display)
- Port C Lower: Input (buttons on PC0, PC1)
- Port C Upper: Output (status LEDs)
```

### 4.2 PPI Initialization Code

```assembly
; Initialize 8255 PPI
; Base Address: 0x40 (Ports: 0x40=A, 0x41=B, 0x42=C, 0x43=Control)

MOV AL, 0x89        ; Control word: PA out, PB out, PC lower in
MOV DX, 0x43        ; Control port address
OUT DX, AL

; Port A: Stepper motor initial state (all coils off)
MOV AL, 0x00
MOV DX, 0x40
OUT DX, AL

; Port B: 7-Segment initial display (position 0 = 0x3F)
MOV AL, 0x3F
MOV DX, 0x41
OUT DX, AL
```

---

## 5. 8254 TIMER CONFIGURATION

### 5.1 Timer Programming

```
Timer 8254 Base Address: 0x60
  Counter 0: 0x60 (Count register)
  Counter 1: 0x61 (Not used)
  Counter 2: 0x62 (Not used)
  Control:   0x63 (Control word)

TARGET: 1-second interrupt
Clock frequency: 1.193 MHz (standard 8088 timer clock)
Count value: 1,193,182 clock cycles = 1 second

Control Word for Counter 0:
0011 0110 (0x36)
│││││││└─ Binary count (0) / BCD (1)
││││││└── Mode 0-5 (010 = Rate Generator/Mode 2)
│││└───── Read/Load (11 = Load Both bytes)
└────────── Counter select (00 = Counter 0)
```

### 5.2 Timer Initialization Code

```assembly
; Initialize 8254 Timer - Counter 0 for 1-second interrupt
MOV AL, 0x36        ; Control word: Counter 0, Mode 2, 16-bit binary
MOV DX, 0x63
OUT DX, AL

; Load count (LSB first, then MSB)
MOV AX, 0x04E2      ; Lower 16 bits of 1,193,182 (0x12345E = 1193182)
MOV DX, 0x60
OUT DX, AL          ; OUT LSB
MOV AL, AH
OUT DX, AL          ; OUT MSB

; Connect timer output to interrupt line (INT3)
; Enable interrupt in 8088 Interrupt Controller
```

---

## 6. INTERRUPT HANDLING

### 6.1 Interrupt Vector Table Setup

```
Interrupt Vector 3 (Timer Interrupt):
  Address: 0x0000C (in Interrupt Vector Table)
  Stores: Far pointer to ISR (Interrupt Service Routine)
  Offset: 4 × 3 = 0x0C

Interrupt Vector 0 (Button/External):
  Address: 0x00000
  Note: Used for manual button interrupt (optional)
```

### 6.2 Interrupt Service Routine (Timer)

```assembly
; ISR for 8254 Timer Interrupt (INT3)
ISR_TIMER:
    PUSH AX
    PUSH DX
    
    ; Rotate motor clockwise (automatic)
    CALL ROTATE_CW
    
    ; Update 7-segment display
    CALL UPDATE_DISPLAY
    
    ; Toggle status LED (PC2)
    MOV DX, 0x42
    IN AL, DX
    XOR AL, 0x04        ; Toggle PC2
    OUT DX, AL
    
    ; EOI (End of Interrupt) - 8088 specific
    MOV AL, 0x20
    MOV DX, 0x20        ; Interrupt Controller port
    OUT DX, AL
    
    POP DX
    POP AX
    IRET
```

---

## 7. PROGRAM FLOW

### 7.1 Main Program Structure

```
START
  │
  ├─→ Initialize 8255 PPI
  ├─→ Initialize 8254 Timer
  ├─→ Setup Interrupt Vectors
  ├─→ Load Stepper Sequence Table
  ├─→ Clear Registers (Position, etc.)
  │
  └─→ MAIN LOOP
       │
       ├─→ Read Button Inputs (Port C)
       │   ├─ Button A pressed? → ROTATE_CW
       │   ├─ Button B pressed? → ROTATE_CCW
       │   └─ No button? → Wait for timer interrupt
       │
       ├─→ Update 7-Segment Display
       ├─→ Check motor position (0-7)
       │
       └─→ Repeat MAIN LOOP

(Timer interrupt occurs every 1 second → Auto CW rotation)
```

### 7.2 Position Counter Logic

```
Current Position: 0-7 (BL register)

ROTATE_CW:
  INC BL
  CMP BL, 0x08
  JNE skip
  MOV BL, 0x00        ; Wrap around at 8
skip:
  (Output stepper sequence)
  CALL UPDATE_DISPLAY

ROTATE_CCW:
  DEC BL
  CMP BL, 0xFF        ; Check if wrapped below 0
  JNE skip
  MOV BL, 0x07        ; Wrap to 7
skip:
  (Output stepper sequence)
  CALL UPDATE_DISPLAY
```

---

## 8. STEPPER SEQUENCE TABLE

```assembly
STEPPER_SEQ:
  DB 0x01   ; Step 0: Coils = 0001
  DB 0x02   ; Step 1: Coils = 0010
  DB 0x04   ; Step 2: Coils = 0100
  DB 0x08   ; Step 3: Coils = 1000

SEGMENT_DISP:
  DB 0x3F   ; Position 0 → Display "0"
  DB 0x06   ; Position 1 → Display "1"
  DB 0x5B   ; Position 2 → Display "2"
  DB 0x4F   ; Position 3 → Display "3"
  DB 0x66   ; Position 4 → Display "4"
  DB 0x6D   ; Position 5 → Display "5"
  DB 0x7D   ; Position 6 → Display "6"
  DB 0x07   ; Position 7 → Display "7"
```

---

## 9. TIMING DIAGRAM

### 9.1 Button Read Timing

```
PC0 (Button A)  ─────┐       ┌──────────────────
                     └───────┘  (Debounce 20ms)

Main Loop:
  Read Port C ────┬────────┬────────┬────────
  Check Button A  │ ✓ YES  │ ✓ YES  │ ✗ NO
  Process Input   │ROTATE  │IGNORE  │WAIT
                  │ CW     │(Debounce)
```

### 9.2 Timer Interrupt Timing

```
Timer Clock (1.193 MHz):
  ├─────────────────────────────────────────────────┐
  │ Count: 1,193,182 cycles                         │
  └─────────────────────────────────────────────────┘
        1 second
        
Interrupt Output:
  OUT0: ┌──────────────┐     ┌──────────────┐
        │              │     │              │
        └──────────────┘     └──────────────┘
        ↑ISR Triggered       ↑ISR Triggered
        (Auto CW rotate)     (Auto CW rotate)
```

---

## 10. CIRCUIT CONNECTIONS SUMMARY

### 10.1 Address/Data Bus

```
8088 CPU          8255 PPI           8254 Timer
───────────       ────────────       ──────────
A0-A7    ═════════ A0-A7             A0-A1
A8-A15   ═════════ (via Decoder)
D0-D7    ═════════ D0-D7  ═══════════ D0-D7
RD       ═════════ RD     ═══════════ RD
WR       ═════════ WR     ═══════════ WR
```

### 10.2 I/O Port Mapping

```
Base Address Assignment:
  8255 PPI:   0x40-0x43
    Port A: 0x40 (Stepper motor)
    Port B: 0x41 (7-segment display)
    Port C: 0x42 (Buttons + Status)
    Control: 0x43
  
  8254 Timer: 0x60-0x63
    Counter 0: 0x60 (1-second timer)
    Control: 0x63
```

---

## 11. DEBOUNCING STRATEGY

### 11.1 Software Debounce (20ms)

```assembly
DEBOUNCE:
  MOV CX, 0xFFFF      ; Delay counter
delay_loop:
  LOOP delay_loop     ; ~20ms delay
  
  ; Read button again
  MOV DX, 0x42
  IN AL, DX
  AND AL, 0x03        ; Mask buttons
  CMP AL, previous_state
  JE valid            ; Stable if same
  JMP DEBOUNCE        ; Not stable, try again
valid:
  RET
```

---

## 12. POWER SEQUENCING

```
Power-up Sequence:
  1. Apply +5V and +12V (stepper motor requires 12V)
  2. 8088 Reset (hold low for 10 µs)
  3. Initialize 8255 PPI
  4. Initialize 8254 Timer
  5. Load interrupt vectors
  6. Clear motor coils (all off)
  7. Display initial position "0"
  8. Enable interrupts (STI)
  9. Enter main loop
```

---

## 13. ERROR HANDLING

### 13.1 Potential Issues & Solutions

```
Issue                          Solution
─────────────────────────────────────────────────
Motor won't start              Check 8255 output
Display shows nothing          Verify 7-segment wiring
Button not responding          Check debounce timing
Timer not generating interrupt Check 8254 count value
Position wraps incorrectly     Verify modulo 8 logic
Coils overheat                 Check power supply
```

---

## 14. SUMMARY TABLE

| Component | Address | Function |
|-----------|---------|----------|
| 8255 Port A | 0x40 | Stepper motor (4 coils) |
| 8255 Port B | 0x41 | 7-segment display (8 segments) |
| 8255 Port C | 0x42 | Buttons (input), LEDs (output) |
| 8255 Control | 0x43 | PPI configuration |
| 8254 Counter 0 | 0x60 | Timer count |
| 8254 Control | 0x63 | Timer configuration |
| Timer Output | INT3 | Interrupt line |
| Button A | PC0 | Clockwise |
| Button B | PC1 | Counter-clockwise |
| Motor Position | BL (reg) | 0-7 (modulo 8) |

---

**Submission Ready:** Complete hardware design with full I/O mapping, timer configuration, interrupt handling, and display logic.
