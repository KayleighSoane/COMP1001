; A simple example adding two numbers.
.386  ; Specify instruction set
.model flat, stdcall  ; Flat memory model, std. calling convention
.stack 4096 ; Reserve stack space
ExitProcess PROTO, dwExitCode: DWORD  ; Exit process prototype


.data ; data segment

array DWORD 10, 20, 30, 40


.code ; code segment

main PROC ; main procedure

mov ecx, 3
mov eax, [array + TYPE array*3]
mul ecx
mov ecx, 6
div ecx
mov [array + TYPE array*3], eax


INVOKE ExitProcess, 0 ; call exit function

main ENDP ; exit main procedure
END main  ; stop assembling 