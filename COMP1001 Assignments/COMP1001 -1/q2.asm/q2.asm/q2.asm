; A simple template for assembly programs.
.386  ; Specify instruction set
.model flat, stdcall  ; Flat memory model, std. calling convention
.stack 4096 ; Reserve stack space
ExitProcess PROTO, dwExitCode: DWORD  ; Exit process prototype


;Tip. You have been taught how to use integer division only. So, implement the division using ‘div’ instruction; assume 
;that the reminder is always zero (we are interested in the quotient only).  
;unsigned int i, B[10], A[10]={3,2,3,1,7,5,0,8,9,2}, C[10]={1,3,2,5,4,6,0,4,5,8}; 
 


;for (i=0; i<10; i++){ 
 ; B[i] = (A[i]*2+1) + (C[i]*3+1) + (A[i]+C[i])/3; 
 ; }   


.data ; data segment
	; define your variables here
B BYTE ?,?,?,?,?,?,?,?,?,?		
A BYTE 3,2,3,1,7,5,0,8,9,2
Ca BYTE 1,3,2,5,4,6,0,4,5,8
D BYTE ?,?,?,?,?,?,?,?,?,?		;D will have value of A+C
 

.code ; code segment

main PROC ; main procedure
	; write your assembly code here


mov esi, 0
segmenta:
mov bl, 2
mov al, [A + TYPE A*esi]
mul bl
mov bl, 1
add al, bl
mov [A + TYPE A*esi], al				;A now has value of *2, +1
inc esi
cmp esi, 10								;loop carried out 10 times
jne segmenta							;A = {7,5,7,3,15,11,1,17,19,5}

mov esi, 0
segmentb:
mov bl, 3
mov al, [Ca + TYPE Ca*esi]
mul bl
mov bl, 1
add al, bl
mov [Ca + TYPE Ca*esi], al				;C now has value of *3, +1
inc esi
cmp esi, 10								;loop carried out 10 times
jne segmentb							;C = {4,10,7,16,13,19,1,13,16,25}

mov esi, 0
segmentc:
mov al, [A + TYPE A*esi]
mov bl, [Ca + TYPE Ca*esi]
add al, bl
mov [D + TYPE D*esi], al				;D now has value of A+C
inc esi
cmp esi, 10
jne segmentc							;D = {11,15,14,19,28,30,2,30,35,30}

mov esi, 0

segmentd:								;segmentd will calculate Darray/3
mov ah, 0
mov cl, 3								;Wasn't working at first - was giving 15/3=175. Figured out ah was affecting the division, needed to clear it every time
mov al, [D + TYPE D*esi]
div cl							
mov [D + TYPE D*esi], al				;D now has value of (A+C)/3 
inc esi									;^^^first value = 11, /3 = 3.6666 - ignore decimals?
cmp esi, 10								;D rounded up = {4,5,5,6,9,10,1,10,12,10}	not rounded up = {3,5,4,6,9,10,0,10,11,10}
jne segmentd							;now have three arrays, need to add together to make B

mov esi, 0
segmente:
mov al, [A + TYPE A*esi]
mov cl, [Ca + TYPE Ca*esi]
mov bl, [D + TYPE D*esi]				;get values from each array
add al, cl							
add al, bl								;add them together
mov [B + TYPE B*esi], al				;B now has value of A+C+D
inc esi									;A: {7,5,7,3,15,11,1,17,19,5} + Ca: {4,10,7,16,13,19,1,13,16,25} + D: {3,5,4,6,9,10,0,10,11,10}
cmp esi, 10								;B = {14,20,18,25,37,40,2,40,46,40} which is   &B: 0e 14 12 19 25 28 02 28 2e 28 
jne segmente

INVOKE ExitProcess, 0 ; call exit function
  
main ENDP ; exit main procedure
END main  ; stop assembling
