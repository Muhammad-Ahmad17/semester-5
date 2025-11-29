;a) Set the leftmost 4 bits of AX
;b) Clear the rightmost 3 bits of AX
;c) Invert the bits 5,7 and 9 of AX.


;MOV AX , 0abcdh
;
;
;OR AX ,  0f000h
;
;AND AX , 0fff7h
;
;XOR AX, 0220H
             
             
             
           ;  Write an assembly language program that clears any bit (from bit0 to bit15) in AX register, leaving
;other bits unchanged. Number of bit that is to be cleared is stored in CL register.
;Hint: If the number 9 is stored in CL register, it means 9th bit of AX should be cleared           
;             
    MOV AX, 0FFFFH      ; Example value
    MOV CL, 9           ; Example: clear bit 9
    MOV BX, 1
    SHL BX, CL          ; Mask = 1 << CL
    NOT BX              ; Invert mask
    AND AX, BX          ; Clear that bit      
    
    
    
    
;
;;Write an assembly language program that counts the number of ‘1’s in a byte residing in CL register.
;;Store the counted number in DH register.
;    
;MOV CL, 10110101B   ; Example byte
;    XOR DH, DH          ; DH = 0 (counter)
;
;    ; Check bit0
;    MOV AL, CL
;    AND AL, 01H
;    ADD DH, AL
;
;    ; Check bit1
;    MOV AL, CL
;    SHR AL, 1
;    AND AL, 01H
;    ADD DH, AL
;
;    ; Check bit2
;    MOV AL, CL
;    SHR AL, 2
;    AND AL, 01H
;    ADD DH, AL
;
;    ; Check bit3
;    MOV AL, CL
;    SHR AL, 3
;    AND AL, 01H
;    ADD DH, AL
;
;    ; Check bit4
;    MOV AL, CL
;    SHR AL, 4
;    AND AL, 01H
;    ADD DH, AL
;
;    ; Check bit5
;    MOV AL, CL
;    SHR AL, 5
;    AND AL, 01H
;    ADD DH, AL
;
;    ; Check bit6
;    MOV AL, CL
;    SHR AL, 6
;    AND AL, 01H
;    ADD DH, AL
;
;    ; Check bit7
;    MOV AL, CL
;    SHR AL, 7
;    AND AL, 01H
;    ADD DH, AL