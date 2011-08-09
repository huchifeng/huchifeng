.386
.model flat, stdcall
option casemap:none

 c_msvcrt typedef PROTO C :VARARG

 externdef _imp__printf:PTR c_msvcrt
    crt_printf equ <_imp__printf>

 includelib /masm32/lib/msvcrt.lib

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

	MASM1111 EQU 5+100
	invoke crt_printf, offset szFormat, MASM1111


	ret

end start

