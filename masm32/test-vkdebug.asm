.386
.model flat, stdcall
option casemap:none

 
 include /masm32/include/masm32rt.inc  
 include \masm32\include\debug.inc
 includelib \masm32\lib\debug.lib


.data
szFormat	db "Hello, World, printf, %d", 10, 0
Message		db "exit now?", 0
TitleTxt	db "title", 0
xxx    db "xxxabc",0

.data?
buffer		db 100 dup(?)

.code
start:

	print "hello", 10
DBGWIN_DEBUG_ON = 0
	PrintDec esp, "Esp value"; 弹出调试窗口显示
DBGWIN_DEBUG_ON = 1
	PrintDec eax
	Fix <I am going to change it later>  ; 编译期显示

	invoke ExitProcess, 0
	ret

end start

