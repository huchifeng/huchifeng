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

	mov eax, 99
	inc eax
	
	.if eax >= 100
	invoke crt_printf, offset szFormat, eax;
	.endif

	mov eax, 105
	.while eax < 110
		inc eax
		push eax ; eax modified by printf
		invoke crt_printf, offset szFormat, eax;
		pop eax
		.if eax >= 108
			.continue .if eax == 108
			.break
		.elseif eax==106
			push eax
			invoke crt_printf, offset szFormat, 106106
			pop eax
		.endif
	.endw

	mov ESI, 1 ; eax, ecx, edx will be changed by printf
	.repeat
		invoke crt_printf, offset szFormat, ESI
		inc ESI
	.until ESI==10

	ret

end start

