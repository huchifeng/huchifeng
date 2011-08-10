.386
.model flat, stdcall
option casemap:none

 c_msvcrt typedef PROTO C :VARARG

 externdef _imp__printf:PTR c_msvcrt
    crt_printf equ <_imp__printf>

 includelib /masm32/lib/msvcrt.lib

 ; how to externdef C++ functions
 call_qDebug_msgType typedef PROTO SYSCALL :DWORD
 externdef qDebug_msgType : PTR call_qDebug_msgType
	qDebug_msgType1 equ <qDebug_msgType> 
 includelib my-qtcore4.lib

 ;externdef ??0QDebug@@QAE@W4QtMsgType@@@Z : PTR call_qDebug_msgType
 ;	qDebug_msgType equ <??0QDebug@@QAE@W4QtMsgType@@@Z>
 ;includelib /qt/4.7.0/lib/qtcore4.lib

.data
szFormat	db "Hello, World, printf, %d", 10

.data?
buffer		db 100 dup(?)
qDebug		dd ?

.code
start:


	invoke crt_printf, offset szFormat, 22299;
	lea ecx, [qDebug]
	invoke qDebug_msgType1, 0
	ret

end start

