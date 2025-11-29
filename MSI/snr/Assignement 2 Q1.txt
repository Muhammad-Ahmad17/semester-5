;a => BL
;x => DL
;y => DH
;j => CL
;k => CH    

MOV BL,252
MOV DL,1
MOV DH,139 
MOV CL,55
MOV CH,122
                                   

FOR:
    MOV AL,BL
    MUL DL       
    MOV DH,AL
    SUB DH,125 
    
    CMP DH,100
    JLE  SKIP
    CALL PAIR
    INC DH
    
    
    SKIP:
    ADD BL,2
    CMP BL,250
    JC  REPEAT
    
    JMP END
    

REPEAT:       
JMP FOR



PAIR:
    MOV AL,CL
    DIV CH
    MOV CL,AL     
   
RET 

END:
