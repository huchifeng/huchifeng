rem C:\masm32\bin\lib.exe
call "C:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
"C:\Program Files\Microsoft Visual Studio 10.0\VC\bin\lib.exe" /machine:ix86 /def:qtcore4.def /out:my-qtcore4.lib
del my*.exp

echo on
pause
exit /b

dumpbin c:/qt/4.7.0/bin/qtcore4.dll /exports > qtcore4.def