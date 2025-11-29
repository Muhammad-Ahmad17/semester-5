; bx = reg.no 
; using division
MOV BX , 113 
MOV AX , BX 
MOV BL , 2 
DIV BL 
MOV DL , AH 
MOV AL , DL  
; o ho we want it in ax
; using xor and checking lsb
MOV BX , 113
MOV AX , BX
AND AX , 0001