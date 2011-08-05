.386
.model flat, stdcall
option casemap:none

include /masm32/include/windows.inc
include /masm32/include/masm32.inc  ; StdOut
 
include /masm32/include/user32.inc
include /masm32/include/kernel32.inc
 
includelib /masm32/lib/user32.lib
includelib /masm32/lib/kernel32.lib
includelib  \masm32\lib\masm32.lib
 
.data
szText      db "Hello, World", 10, 0 ; \n not supported

.data?
buffer db 100 dup(?)
 
.code
start:

MyMacro macro t
	invoke StdOut, ADDR t
endm

	;invoke printf, offset szText;
	jmp @F
	invoke OutputDebugStringA, offset szText;
@@:
	MyMacro szText
	invoke StdOut, ADDR szText
    invoke StdIn,ADDR buffer,100
       
    invoke ExitProcess, 0
end start

