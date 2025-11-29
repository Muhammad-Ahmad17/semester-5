; mul 8-bit 
; command : mul bl
; final result store in ax
; ax = <args> *al  where args in bl
; steps : 
   ; a = 5 , b = 6 , c = a*b
   ; mov on of the args in al 
   ; mov other args in bl
   ; mul bl
   ; check ax which is equal to result
    
;mov al , 8
;mov bl , 2
;mul bl   

;====>  ax = bl * al                      
  
; =>  Q # 2    calcuklate the squre of a number lies in 8 bit      
;mov dl , 7  
;mov al , dl
;mov bl , 7
;mul bl
;mul bl  
     

; mul 16-bit 
; command : mul bx  
; dxax = <args> * ax where args in bx

; => Q # 2 Extended
; mov dx , 0feh
; mov ax , dx 
; mov bx , 0feh 
; mul bx
; mul bx  
; 
 
 ; practise  ax = bl * al | dxax = bx * ax 
 ; mul 8 bit * 8 bit *16 bit  
; ; 17 * 225 * 
; mov al , 0ffh
; mov bl , 0ffh    
; mul bl 
; ; now bx is containg the result which we have to multiply with the new 16 bit number 
; 
; mov bx , 0ffffh
; mul bx 
 
 


          
;mov al, 0ffh        ; AL = 255
;mov bl, 0ffh        ; BL = 255
;mul bl              ; AX = 255 * 255 = 65025
;
;mov bx, 0ffffh      ; BX = 65535
;mul bx              ; DX:AX = AX * BX = 65025 * 65535
;
 
   
   
                       
                       
                       
    ;                                                                       ;   == > we mul two 8 bit number and then a 16 bit 
;                       
;       ;   0bh *  011h  *  03e8h       ax = bl * al  | dxax = bx * ax
;       mov  al , 0bfh
;       mov  bl , 011h
;       mul  bl
;       mov  bx , 03e8fh
;       mul  bx  
;       
       

    ;       mov ax , 0034
;           mov bl , 006
;           div bl  



; Q NO 3            
;             ;5/9 * (f-32)             c =ah , f = al
;;   
;   mov al , 100 ; f = 100
;   mov bl , 32  ; bl = 32
;   sub al , bl  ; al = al - 32 (al = 100 - 32)
;   mov dl , al  ; dl = al 
;   
;   mov ax , 5
;   mov bl , 9 
;   div bl ; al = quot , ah = rem
;   
;   ; mov al , quot -> already
;     mov bl , dl 
;     mul bl ; bl = ans 
;     mov ah , bl
                
                
                
                
                
                
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
