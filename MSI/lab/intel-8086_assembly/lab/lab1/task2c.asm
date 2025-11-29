		   
	; Part c: Swap ABCDH:2345H and 1234H:78DEH  without XCHG     
	; load  ABCDH: 2345H into bx
	mov ax , 0abcdh
	mov ds , ax                      
	mov [2345h],44h  ; hamara kaam
	mov bx,[2345h]  ; bx = 44h
	; load  1234H: 78DEH into cx
	mov ax , 1234h
	mov ds , ax  
	mov [78DEH],45h  ; hamara kaam
	mov cx , [78DEH] ; 45h

	; swap bx and cx 
	mov dx , bx
	mov bx , cx
	mov cx , dx

	; now store value back to memory 

	mov ax , 0abcdh
	mov ds , ax 
	mov [2345h],bx 

	mov ax , 1234h
	mov ds , ax  
	mov [78DEH],cx

	; now load back to verify 
	; load  ABCDH: 2345H into bx
	mov ax , 0abcdh
	mov ds , ax
	mov bx,[2345h]

	; load  1234H: 78DEH into cx
	mov ax , 1234h
	mov ds , ax  
	mov cx , [78DEH]
					 
					 
	;  ABCDH:2345H : 44H
	;  1234H:78DEH : 45H
					 
