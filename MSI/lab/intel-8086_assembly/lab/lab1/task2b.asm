; Part b: Swap SS and DS without XCHG
; Assume BX is available as second temp
MOV BX, DS          
MOV AX, SS          ; NOTE: MOV SS, DS ; Move DS to SS (WE ARE UNABLE TO DO THAT IT CAN BE DONE DURING EXECUTION
MOV SS, BX  
;mov bx , ax
MOV DS, AX   
