; Part A:swap SP and DI without XCHG
MOV AX, SP
MOV SP, DI
MOV DI, AX
