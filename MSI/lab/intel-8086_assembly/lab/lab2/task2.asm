; 5/9 * (F - 32)    
; C = AH , F = AL    

    mov al , 100     ; f = 100
    mov bl , 32      
    sub al , bl      ; al = al - 32  (F - 32)

    mov dl , al      ; save (F - 32)

    mov al , dl      
    mov bl , 5       
    mul bl           ; AX = (F-32) * 5

    mov bl , 9       
    div bl           ; AL = quotient ( (F-32)*5 / 9 ), AH = remainder

    mov ah , al      ; Celsius -> AH
