set path=D:\huchifeng.tmp\swigwin-2.0.4;%path%
call "%ProgramFiles%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
echo on

rem csc /? && pause
rem cl /? & pause
rem swig --help

swig -csharp example.i
cl  example.c example_wrap.c /Fe"example.dll" /LD

csc /out:example.exe examplePINVOKE.cs runme.cs example.cs
example.exe

pause

del *.dll
del *.obj
del *.exp
del *.lib
del *_wrap.c
del *PINVOKE.cs
del *.exe
del example.cs
dir

pause
