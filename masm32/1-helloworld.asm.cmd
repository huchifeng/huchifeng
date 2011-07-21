
; hello world

comment # >nul 2>&1
    rem @echo off
    echo hello
    set path=c:\masm32\bin;%systemroot%
    del *.obj
    ml /c /coff %0
    link /SUBSYSTEM:WINDOWS /OPT:NOREF %~n0.obj
    %~n0.exe
    exit /b
# ; delimiter only one char, ~ not OK; ^ # OK
 


; HelloWorld.asm
 
comment * ----------------------------------------------
                 The First Assemble Application
                ---------------------------------------------- *
 
.386
.model flat, stdcall
option casemap:none

include /masm32/include/windows.inc
 
include /masm32/include/user32.inc
include /masm32/include/kernel32.inc
 
includelib /masm32/lib/user32.lib
includelib /masm32/lib/kernel32.lib
 
.data
szCaption   db 'title', 0
szText      db 'Hello, World!', 0
 
.code
start:
    invoke MessageBox,   ; function
          NULL,          ; parent handle
          offset szText,       ; text
          offset szCaption,    ; caption
          MB_OK                    ; ∞¥≈•¿‡–Õ
       
    invoke ExitProcess, 0
end start

