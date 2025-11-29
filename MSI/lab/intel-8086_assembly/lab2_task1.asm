;mov al , 5 
;mov bl , 3 
;sub bl , al ; 3-5 => Cflag is set to one and one borow to bl
;            
;  ;1(carry)    0000 0011
;            ;  0000 0101
;            ;------------
;            ;- 1111 1110


mov al , 7
mov bl , 7
mul bl ; ax = <bl> * al // ax=49 

mul ax;  ax = <ax> * al // ax=7*49


MOV AL, 77   ; F 
SUB AL, 32        
MOV BL, 5
MUL BL            
MOV BL, 9
DIV BL            
MOV AH, AL  
