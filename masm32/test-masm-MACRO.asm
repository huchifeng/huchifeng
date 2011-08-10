.386
.model flat, stdcall
option casemap:none

 c_msvcrt typedef PROTO C :VARARG

 externdef _imp__printf:PTR c_msvcrt
    crt_printf equ <_imp__printf>

 includelib /masm32/lib/msvcrt.lib

Macro1 MACRO x:REQ
%ECHO <Number &x&x &x&&@Date @Time> ; 
EXITM
ENDM

Macro2 MACRO
EXITM <200> ; 必须是 text
ENDM

Macro3 MACRO x
EXITM @SizeStr(x)
ENDM



.data
szFormat	db "Hello, World, printf, %d", 10

Msft EQU <Microsoft>


.data?
buffer		db 100 dup(?)

.code
start:

	% FOR arg:REQ, <1, 2, 3 >
		IF arg EQ 2
			invoke crt_printf, offset szFormat, arg
			invoke crt_printf, offset szFormat, arg
		ELSEIF arg NE 2
			invoke crt_printf, offset szFormat, arg
		ENDIF
	ENDM

	FORC arg, <456>
	  invoke crt_printf, offset szFormat, arg
	ENDM

	FOR arg, <1,2>
	;GOTO Crash ; only allowed in MACRO,FOR,FORC,REPEAT,WHILE
	:Crash
	;.ERR <Undefined symbol.>
	ENDM

	MASM EQU 5+100
	invoke crt_printf, offset szFormat, MASM

	FORC arg, Msft
	   ECHO arg ; 只显示编译信息 m s f t
	ENDM

	ECHO echo a b c, d
	% ECHO Assembled on @Date at @Time ; 加 % 否则不插入 Date Time 变量
	%OUT abc def ; included for compatibility

	Macro1 <abcd>
	; Macro1 ;  missing parameter

	invoke crt_printf, offset szFormat, Macro2(); 得到返回值
	%ECHO Macro2 Macro2 () Macro2; 加 % 及() 才调用 Macro
	%ECHO Macro3(hello-world); // 得到长度 011

	ret

end start

