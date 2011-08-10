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
      include \masm32\include\DateTime.inc
	  includelib \masm32\lib\DateTime.lib

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

    .data?
		pszDateTime		db 100 dup(?)

	.data
		szFormat db "current date time:%s", 10, 0
		xxxx db 49,0

    .code

; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

start:

	call main
	invoke ExitProcess,eax

main proc
	LOCAL pdt :DATETIME
	INVOKE GetLocalDateTime, ADDR pdt
	INVOKE DateTimeToString, ADDR pdt, offset pszDateTime
	invoke crt_printf, offset szFormat, offset pszDateTime
	ret
main endp

; <<<<<<<<<<<<<<<<<<
end start