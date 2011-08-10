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
align 4
szText      db "Hello, World", 10, 0 ; \n not supported
szFormat	BYTE "Hello, World, printf, %d ", 3 dup(33), 10, 0;  0 不可少！
;   BYTE(DB) WORD(DW) DWORD=REAL4(DD) FWORD(DF,6byte) QWORD=REAL8(DQ), TBYTE=REAL10(DT)

.data?  ; uninitialised data section
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

; __stdcall调用约定：函数的参数自右向左通过栈传递，被调用的函数在返回前清理传送参数的内存栈，
;	后面加上一个"@"符号和其参数的字节数，格式为_functionname@number
;	例如 ：function(int a, int b)，其修饰名为：_function@8 
;   user32.dll 中的函数都没有 _ 和 @n ？
; _cdecl是C和C＋＋程序的缺省调用方式。每一个调用它的函数都包含清空堆栈的代码，
;		所以产生的可执行文件大小会比调用_stdcall函数的大。
;		函数采用从右到左的压栈方式。
;		注意：对于可变参数的成员函数，始终使用__cdecl的转换方式。 
;	仅在输出函数名前加上一个下划线前缀，_functionname
; __fastcall调用约定：它是通过寄存器来传送参数的
;		（实际上，它用ECX和EDX传送前两个双字（DWORD）或更小的参数，剩下的参数仍旧自右向左压栈传送，
;		被调用的函数在返回前清理传送参数的内存栈）。 
;	输出函数名前加上一个"@"符号，后面也是一个"@"符号和其参数的字节数，格式为@functionname@number。
; thiscall仅仅应用于"C++"成员函数。this指针存放于CX寄存器，参数从右到左压。
;		thiscall不是关键词，因此不能被程序员指定。 
; naked call; 
;	其他调用约定，如果必要的话，进入函数时编译器会产生代码来保存ESI，EDI，EBX，EBP寄存器，
;		退出函数时则产生代码恢复这些寄存器的内容。
;	naked call不产生这样的代码。
;	naked call不是类型修饰符，故必须和_declspec共同使用。

; 对于导出的C++类，仅能使用__cdecl调用约定
;   ??4QUrl@@QAEAAV0@ABVQString@@@Z  ; 构造函数，QAE 固定, AAV 是引用参数， ABV 常数引用， @Z 结束
; 对于导出的C++类中的成员函数（非构造函数和析构函数），可以使用不同的调用约定
;   方法名在前
;  ?size@QString@@QBEHXZ
;  ?sleep@QThread@@KAXK@Z


	invoke crt_printf, offset szFormat, 9999; //error A2006: undefined symbol : printf

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

