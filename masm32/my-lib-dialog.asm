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

    .data?
        hInstance dd ?

    .code

; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



;<<<<<<<<<<<<<<<<<<<<<<

call_sample_dialog proc

    ; Dialog 等 是宏，执行了 GlobalAlloc,  MultiByteToWideChar 等操作
    Dialog "Bare Bones Dialog", \           ; caption
           "MS Sans Serif",10, \            ; font,pointsize
            WS_OVERLAPPED or \              ; styles for
            WS_SYSMENU or DS_CENTER, \      ; dialog window
            2, \                            ; number of controls
            50,50,150,80, \                 ; x y co-ordinates
            1024                            ; memory buffer size

    DlgButton "Cancel",WS_TABSTOP,48,40,50,15,IDCANCEL
    DlgStatic "Bare Bones Dialog Written In MASM32", \
              SS_CENTER,2,20,140,9,100

    CallModalDialog hInstance,0,dlgproc,NULL

    ret

call_sample_dialog endp

; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

dlgproc proc hWin:DWORD,uMsg:DWORD,wParam:DWORD,lParam:DWORD

    LOCAL hDC :DWORD
    LOCAL ps :PAINTSTRUCT
    LOCAL rct :RECT

    .if uMsg == WM_INITDIALOG
      invoke SendMessage,hWin,WM_SETICON,1,
                         FUNC(LoadIcon,NULL,IDI_ASTERISK)

    .elseif uMsg == WM_COMMAND
      .if wParam == IDCANCEL
        jmp quit_dialog
      .endif

    .elseif uMsg == WM_CLOSE
      quit_dialog:
      invoke EndDialog,hWin,0

    .elseif uMsg == WM_PAINT ;
      invoke BeginPaint,hWin,ADDR ps
      mov hDC, eax
      invoke GetClientRect,hWin,ADDR rct
      invoke DrawEdge,hDC,ADDR rct,EDGE_ETCHED,BF_RECT
	  invoke Ellipse,hDC, rct.left, rct.top, rct.right, rct.bottom
      invoke EndPaint,hWin,ADDR ps

    .elseif uMsg == WM_ERASEBKGND and 0
      ;mov eax,1
	  ;ret
	.elseif uMsg ==  WM_CTLCOLORDLG
		invoke CreateSolidBrush,00FFAAAAh ; blue
		ret
    .endif

    xor eax, eax
    ret

dlgproc endp

; <<<<<<<<<<<<<<<<<<
end