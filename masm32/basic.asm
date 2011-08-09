.386
.model flat, stdcall
option casemap:none

include /masm32/include/windows.inc
include /masm32/include/masm32.inc  ; StdOut
include /masm32/include/msvcrt.inc  ; printf
 
include /masm32/include/user32.inc
include /masm32/include/kernel32.inc
 
includelib /masm32/lib/user32.lib
includelib /masm32/lib/kernel32.lib
includelib /masm32/lib/masm32.lib
includelib /masm32/lib/msvcrt.lib ; printf

myst STRUCT
    left    DWORD  ?
    top     DWORD  ?
    right   DWORD  ?
    bottom  DWORD  ?
myst ENDS

myst2 STRUCT
    a myst <>
	b myst <>
myst2 ENDS
 
.data
szText      db "Hello, World", 10, 0 ; \n not supported
szFormat	db "Hello, World, printf, %d", 10

.data?
buffer		db 100 dup(?)



 
.code
start:

MyMacro macro t
	invoke StdOut, ADDR t
endm

; use vc debug, show disassembly

	mov AL, 100
	AAA
	mov AH, 2
	AAD

; AF CF OF SF PF ZF
; 辅助进位标志AF(Auxiliary Carry Flag)
;   (1)、在字操作时，发生低字节向高字节进位或借位时；
;	(2)、在字节操作时，发生低4位向高4位进位或借位时。
; 进位标志CF(Carry Flag)
;   多字(字节)数的加减运算，无符号数的大小比较运算，移位操作，字(字节)之间移位，专门改变CF值的指令等。
; 溢出标志OF(Overflow Flag
;	有符号数加减运算所得结果是否溢出
; 符号标志SF(Sign Flag)
;	反映运算结果的符号位，它与运算结果的最高位相同
; 奇偶标志PF(Parity Flag)
;   运算结果中“1”的个数的奇偶性。如果“1”的个数为偶数，则PF的值为1，否则其值为0。
; 零标志ZF(Zero Flag
;   如果运算结果为0，则其值为1，否则其值为0。

; imm8 imm16 imm32 ;; immediate 32bit ; 立即数
; r/m32 ; register or memory
; /0：代表此OpCode存在ModR/M结构（后面有讲）
; /r： 代表此OpCode存在ModR/M结构（后面有讲）

; Prefixes： 前缀（最多4个前缀，每个1字节，并不是必需的）
; code：主操作码（1-3字节不等）
; ModR/M：固定 1字节大小，并不是必需的
; SIB：固定1字节大小，并不是必需的
; Displacement：偏移量（1、 2、4字节，并不是必需的）
; Immediate：立即数（1、2、4字节，并不是必需的）

; —F0H—LOCK prefix.
; —F2H—REPNE/REPNZ prefix (used only with string instructions)
; —F3H—REP prefix (used only with string instructions).
; —F3H—REPE/REPZ prefix (used only with string instructions).
; —F3H—Streaming SIMD Extensions prefix.

ADD EBX, 10000  ; // 81 C3 id
ADD ECX, 100 ; // 83 C1 ib
ADC ECX, 100 ; // 83 D1 64 
;ADD [EAX], 10000;
ADD AH,2; // 80 C4 02
ADD AH,AL; // 02 E0
; call AX; // 66 FF D0 
; call eax; // FF D0  // FF /2


	invoke crt_printf, offset szFormat, 99; //error A2006: undefined symbol : printf

	invoke OutputDebugStringA, offset szText; // OK
	MyMacro szText
	invoke StdOut, ADDR szText

    mov [buffer+0], "1"
    mov [buffer+1], 10
	mov [buffer+2], 0
	mov eax, offset buffer
@@:    
    invoke StdOut, ADDR buffer
    inc [buffer+0]
	cmp [buffer+0], "6"
	je @F
	jmp @B

	; old dos insrtuctions, slow
	; LOOP, LOOPE, LOOPZ, LOOPNZ, LOOPNE
	; MOVS, LODS, CMPS, SCAS
	; MOVSD, LODSB etc ...
	; JCXZ should be replaced with,  
	;		test ecx, ecx
	;		je StartLabel

@@: 
	call myproc



    ;invoke StdIn,ADDR buffer,100 ; getchar
	;invoke StdOut,ADDR buffer ; its ok
    invoke ExitProcess, 0


	;windows convention:
	; ESP and EBP are usually used for constructing a stack frame 
	; should keep unchange EBX ESI EDI
	; can freely modify EAX ECX EDX

	myproc proc
	  LOCAL myst1 :myst2;
	  LOCAL myi32 :DWORD;

	  mov myst1.a.left, 0
	  
	  ASSUME eax:PTR myst2
      ; mov eax, ADDR myst1 ; //cannot get ADDR myst1, for its LOCAL; use LEA; invoke use LEA too;
	  lea eax, myst1
      mov [eax].a.left, 7
      ASSUME eax:nothing
	  ;mov (myst2 PTR [eax]).a.left, 7

	  mov eax, offset szText
	  add eax, myst1.a.left
	  invoke StdOut, eax 
	  ret
	myproc endp

end start

