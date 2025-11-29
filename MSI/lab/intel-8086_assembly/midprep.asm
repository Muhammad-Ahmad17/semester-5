; ; > read char
;MOV AH, 07h     ; Read char without echo | 2 for echo
;INT 21h         ; AL = ASCII of key (e.g. '2' = 32h)
; ; > disp char
;MOV DL, AL      ; copy AL > DL
;MOV AH, 02h     ; Display function
;INT 21h         ; displays the character in DL  
;
;;MOV AH, 1
;;INT 21h   ; al 
;
;
;	MOV AH, 2
;	MOV DL, 5   
;	ADD DL , 30h
;	INT 21h
;
;MOV AH, 1
;INT 21h
;       
;MOV CH, AL
;
;MOV AH , 1
;INT 21h
;
;MOV  CL,AL  
;
;    MOV AH, 2
;	MOV DL, CH  
;	;ADD DL , 30h
;	INT 21h   
;	MOV AH, 2
;	MOV DL, CL  
;	;ADD DL , 30h
;;	INT 21h   
;	   
;	   MOV CH , 1 
;	   MOV CL, 2
;	   
;	
;	ADD CH , CL 
;	
;    MOV AH, 2
;	MOV DL, CH  
;	ADD DL , 30h
;	INT 21h  
;           
; 
		ORG 100h  
		
		.data 
		    msg DB "enter number from 1 to 99: $" 
	        nl DB 0ah , 0dh ,"$ "
		    msg1 DB "the number $"
		    notdiv DB "is not divisible by $ "
		    isdiv DB "is divisible by $ "  
		    isand DB "and $"

		.code
		
		MOV CX , @data 
		MOV DS , CX
		
		LEA DX,  msg
		MOV AH, 9
		INT 21h  
		
        MOV AH, 1
        INT 21h
               
        MOV CH, AL
        
        MOV AH , 1
        INT 21h
        
        MOV  CL,AL 
        
        LEA DX,  nl
		MOV AH, 9
		INT 21h
		
		LEA DX,  msg1
		MOV AH, 9
		INT 21h	 
		
		MOV AH, 2
    	MOV DL, CH
    	INT 21h

		MOV AH, 2
    	MOV DL, CL
    	INT 21h  
    	            
    	            
    	           

    	MOV AL , CH
    	SUB AL , 30h ; mul 10 
    	MOV BL, 10      ; multiplier = 10
        MUL BL          ; AL * BL ? AX (result in AX)

    	MOV AH , CL
    	SUB AL , 30h   
    	        
    	        ADD AL , AH
    	        XOR AH , AH 
    	MOV CL , 12   
    	DIV CL
    	;AH = rem 
    	          
    	          
    	CMP AH , 0 
    	JZ yes
    	JMP no
    	
    	yes:
    	LEA DX,  isdiv
		MOV AH, 9
		INT 21h 
		RET
    	
    	no:
    	LEA DX,  notdiv
		MOV AH, 9
		INT 21h 
    	
    	  
  		