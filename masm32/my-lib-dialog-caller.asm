;  // uniocode, UTF-8 的 BOM 导致编译问题  invalid character in file

      .486                      ; create 32 bit code
      .model flat, stdcall      ; 32 bit memory model
      option casemap :none      ; case sensitive

;     include files
;     ~~~~~~~~~~~~~
      include \masm32\include\windows.inc
      include \masm32\include\masm32.inc
      include \masm32\include\gdi32.inc
      include \masm32\include\user32.inc
      include \masm32\include\kernel32.inc
      include \masm32\include\Comctl32.inc
      include \masm32\include\comdlg32.inc
      include \masm32\include\shell32.inc
      include \masm32\include\oleaut32.inc

;     libraries
;     ~~~~~~~~~
      includelib \masm32\lib\masm32.lib

      includelib \masm32\lib\gdi32.lib
      includelib \masm32\lib\user32.lib
      includelib \masm32\lib\kernel32.lib
      includelib \masm32\lib\Comctl32.lib
      includelib \masm32\lib\comdlg32.lib
      includelib \masm32\lib\shell32.lib
      includelib \masm32\lib\oleaut32.lib

      FUNC MACRO parameters:VARARG
        invoke parameters
        EXITM <eax>
      ENDM

      include \masm32\include\dialogs.inc

      dlgproc PROTO :DWORD,:DWORD,:DWORD,:DWORD

	  include /masm32/include/msvcrt.inc  ; printf
	  includelib /masm32/lib/msvcrt.lib ; printf

	  call_sample_dialog PROTO
	  includelib my-lib-dialog.lib

    .data?
        hInstance dd ?

	.data
		szFormat	db "Hello, World, printf, %d", 10

    .code

; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

start:

	call main
	invoke ExitProcess,eax

main proc
	invoke crt_printf, offset szFormat, 99;
	invoke call_sample_dialog
	ret
main endp

; <<<<<<<<<<<<<<<<<<
end start