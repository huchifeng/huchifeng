.386
.model flat, stdcall
option casemap:none

 c_msvcrt typedef PROTO C :VARARG

 externdef _imp__printf:PTR c_msvcrt
    crt_printf equ <_imp__printf>
 
 include /masm32/include/masm32rt.inc
 includelib /masm32/lib/msvcrt.lib

 ; how to externdef C++ functions
 ;call_qDebug_msgType typedef PROTO C :DWORD
 ;externdef _imp_??0QDebug@@QAE@W4QtMsgType@@@Z : PTR call_qDebug_msgType

 ;call_qDebug_out_int typedef PROTO C :DWORD
 ;externdef _imp_??6QDebug@@QAEAAV0@H@Z : PTR call_qDebug_out_int

 zlibVersion PROTO C
 ??0QDebug@@QAE@W4QtMsgType@@@Z PROTO SYSCALL :DWORD
 ??6QDebug@@QAEAAV0@H@Z PROTO SYSCALL :DWORD
 ??1QDebug@@QAE@XZ PROTO SYSCALL
 qDebug_msgType equ <??0QDebug@@QAE@W4QtMsgType@@@Z>
 qDebug_out_int equ <??6QDebug@@QAEAAV0@H@Z>
 qDebug_destroy equ <??1QDebug@@QAE@XZ>

 includelib /qt/4.7.0/lib/qtcore4.lib

.data
szFormat	db "Hello, World, printf, %d", 10

.data?
buffer		db 100 dup(?)
qDebug		dd ?

.code
start:

	invoke zlibVersion
	print eax
	print "=",10

	lea ecx, [qDebug]
	invoke qDebug_msgType, 0

	lea ecx, [qDebug]
	invoke qDebug_out_int, 987654321

	lea ecx, [qDebug]
	call qDebug_destroy ; the real output here !

	invoke crt_printf, offset szFormat, 123;

	invoke ExitProcess, 0

end start

