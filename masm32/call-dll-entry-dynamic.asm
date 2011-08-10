.386
.model flat, stdcall
option casemap:none

 c_msvcrt typedef PROTO C :VARARG

 externdef _imp__printf:PTR c_msvcrt
    crt_printf equ <_imp__printf>

 includelib /masm32/lib/msvcrt.lib
 include /masm32/include/masm32rt.inc

.data
szFormat	db "Hello, World, printf, %d", 10
hLib				dd	0
pProc				dd	0
p_QDebug_MsgType	dd	0
p_QDebug_out_int	dd	0
p_QDebug_destroy	dd	0

.data?
buffer				db 100 dup(?)
qDebug				dd ?

.code
start:

	mov hLib, rv(LoadLibraryA, "QtCore4.dll")
	print "hLib:";
	print str$(hLib), 32
	print uhex$(hLib), 10

	mov pProc, rv(GetProcAddress, hLib, "zlibVersion")
	print "pProc:";
	print uhex$(pProc), 10

	;call dword ptr[pProc]
	call pProc; // same
	mov edi, eax
	print "zlibVersion:"
	print edi, 10

	; 使用 depends 可以 Undecorate C++ functions
	mov p_QDebug_MsgType, rv(GetProcAddress, hLib, "??0QDebug@@QAE@W4QtMsgType@@@Z")
	mov p_QDebug_out_int, rv(GetProcAddress, hLib, "??6QDebug@@QAEAAV0@H@Z")
	mov p_QDebug_destroy, rv(GetProcAddress, hLib, "??1QDebug@@QAE@XZ")
	print uhex$(p_QDebug_MsgType), 10
	print uhex$(p_QDebug_out_int), 10
	print uhex$(p_QDebug_destroy), 10

	push 0
	lea ecx, [qDebug]
	call p_QDebug_MsgType

	push 63h
	lea ecx, [qDebug]
	call p_QDebug_out_int

	lea ecx, [qDebug]
	call p_QDebug_destroy

	invoke ExitProcess, 0
	ret

end start

