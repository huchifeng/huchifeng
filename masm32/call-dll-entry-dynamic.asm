.386
.model flat, stdcall
option casemap:none

 c_msvcrt typedef PROTO C :VARARG

 externdef _imp__printf:PTR c_msvcrt
    crt_printf equ <_imp__printf>

 includelib /masm32/lib/msvcrt.lib

.data
szFormat	db "Hello, World, printf, %d", 10

.data?
buffer		db 100 dup(?)

.code
start:


	invoke crt_printf, offset szFormat, 22299;
	ret

end start

