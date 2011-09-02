set path=D:\huchifeng.tmp\swigwin-2.0.4;%path%
set path=%ProgramFiles%\Microsoft Visual Studio 10.0\VC;%path%;
set path=%ProgramFiles(x86)%\Microsoft Visual Studio 10.0\VC;%path%;
call vcvarsall.bat
echo on

rem csc /? && pause
rem cl /? & pause
rem swig --help

swig -csharp example.i
cl  example.c example_wrap.c /Fe"example.dll" /LD

csc /platform:x86 /out:example.exe examplePINVOKE.cs runme.cs example.cs
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
exit /b


 System.BadImageFormatException
可能是 64位环境运行 32位
需要指定 
csc /platform:x86


