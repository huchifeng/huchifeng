.386
.model flat, stdcall
option casemap:none

 c_msvcrt typedef PROTO C :VARARG

 externdef _imp__printf:PTR c_msvcrt
    crt_printf equ <_imp__printf>

 includelib /masm32/lib/msvcrt.lib
 include /masm32/include/masm32rt.inc  ; fn rv 等 MACRO ?

.data
szFormat	db "Hello, World, printf, %d", 10, 0
Message		db "exit now?", 0
TitleTxt	db "title", 0
xxx    db "xxxabc",0

.data?
buffer		db 100 dup(?)

.code
start:

  print cfm$("\tThis is a test\n\tof the \qfmt$\q macro\n\l\r\x\a\b\n")
  ; 运行显示; 与C语法不同

  print str$(100),"=10*10", 10
  print "path="  ; 只能分为两句
  print env$("path"), 10  ; 10不能用cfm$

  switch 1
    case 1
	  fn crt_printf, "switch test ok %d%c", 0, 10;
	default
  endsw

   switchi$ offset xxx; 需要零结尾！
	 casei$ "xxxABC" ; 大小写忽略模式
	   fnc crt_printf, "switchi$ test ok %d\n", 0
	   ; 不需要C语言的 break
	   ; fnc 比 fn , use quoted text with C style escape characters
	 elsei$
	   fn crt_printf, "switchi$ test fail", 0, 10
  endswi$

	cmp FUNC(MessageBox,0,ADDR Message,ADDR TitleTxt,MB_YESNOCANCEL), IDYES
	je the_end

	fn MessageBox,0,"Hi, I am a Message Box","Greetings",MB_OK
	; fn 使得常量不必 db 声明
	mov edx, rv(MessageBox,NULL,"Message Text for MessageBox","Title Text",MB_OK)
	; rv 代替 FUNC
	
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

the_end:
	invoke ExitProcess, 0
	ret

end start

