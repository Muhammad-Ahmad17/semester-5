;    MOV CL, 10110101B   ; Example byte              
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
;
;;    ; Now DH contains count of 1s
;;    MOV AH, 4CH
;;    INT 21H

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                       
    MOV AX, 000fH       

    ; a) Set the leftmost 4 bits of AX
    OR AX, 0F000H
    ; b) Clear the rightmost 3 bits of AX
    AND AX, 0FFF8H
    ; c) Invert the bits 5,7 AND 9 of AX.  101010000
    XOR AX, 02A0H    

 