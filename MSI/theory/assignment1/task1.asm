.MODEL SMALL
.STACK 100h
.DATA
; Variables and constants are defined here 
    oper1 DB 0ah , 0dh , 'enter operand 1: $'
    oper2 DB 0ah , 0dh , 'enter operand 2: $'
    oper3 DB 0ah , 0dh , 'enter operator {+,-,*,/} : $'
    div0 DB 0ah , 0dh , 'division by zero $'
    res DB 0ah , 0dh , 'result: $'
    agn DB , 0dh , 0ah , 'want to continoue?(y/n):$'
    
.CODE
MAIN PROC
    start: 
    ; initialinzing data mem.     
    MOV CX, @data
    MOV DS ,CX 
    
    
    ; show prompt  for oper-1
    LEA DX , oper1 
    MOV AH , 09h
    INT 21h
    ; read oper-1
    MOV AH, 01h     ; Read char without echo
    INT 21h         ; al = read data 
    SUB AL, 30h
    MOV CL , AL
    ; sub cl , 30h    ; cl = operand 1
    
    
    ; show prompt  for oper-2
    LEA DX , oper2 
    MOV AH , 09h
    INT 21h
    ; read oper-2
    MOV AH, 01h     ; Read char without echo 
    INT 21h         ; al = read data
    SUB AL, 30h   
    MOV CH , AL
    ;sub ch , 30h    ; ch = opernad 2  
           
           
    ; show prompt  for oper-2
    LEA DX , oper3 
    MOV AH , 09h
    INT 21h
    ; read oper-3
    MOV AH, 01h     ; Read char without echo 
    INT 21h         ; al = read data   
    MOV BH , AL     ; bh = operator 3
    
    ; call calculator
    CALL CALCULATOR  ; input : [cl:operand-1 , ch:operand-2 , bh:operator] , output :[cl:result]  
    
    ; show prompt  for result
    LEA DX , res 
    MOV AH , 09h
    INT 21h
    
    ; show result 
    ADD CL, 30h
    MOV DL, CL      ; copy AL > DL
    MOV AH, 02h     ; Display function
    INT 21h         ; displays the character in DL
    
    ;again ? 
    ; prompt user to rerun or exit
    LEA DX, agn
    MOV AH, 09h
    INT 21h
    ; read user choice
    MOV AH, 01h     
    INT 21h
    CMP AL, 'Y'
    JE start
    CMP AL, 'y'
    JE start
     
    ; return to terminal
    MOV AH, 4Ch       ; return to DOS
    INT 21h     
MAIN ENDP
; Procedures other than MAIN will be written here 
CALCULATOR PROC 
    
    CMP BH, '+'    ; ASCII 2Bh
    JE ADDITION

    CMP BH, '-'    ; ASCII 2Dh
    JE SUBTRACTION

    CMP BH, '*'    ; ASCII 2Ah
    JE MULTIPLICATION

    CMP BH, '/'    ; ASCII 2Fh
    JE DIVISION
    RET 
    ADDITION:
        ADD CL , CH    ; res = cl 
        RET
    SUBTRACTION: 
        SUB CL , CH    ; res = cl
        RET
        
    MULTIPLICATION:   ; ax = al * <>
        MOV AL , CL
        MUL CH        ; res = ax  
        MOV CL , AL   ; only taking lsbs
        RET
    
    DIVISION: 
        ; div by 0
        XOR AX, AX  ; clear garbage
        CMP CH , 0
        JE ERROR        
        
        MOV AL , CL  ; ax = ah:al
        DIV CH     ; res = ah 
        MOV CL , AL
        RET            
        
    ERROR:
        ; show prompt  for oper-1
        LEA DX , div0 
        MOV AH , 09h
        INT 21h 
        ; placeholder result
        MOV CL , '(' ; ( + 30h = X
        RET              
CALCULATOR ENDP
END MAIN
   
