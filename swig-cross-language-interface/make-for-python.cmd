set path=D:\huchifeng.tmp\swigwin-2.0.4;%path%
set path=%ProgramFiles%\Microsoft Visual Studio 10.0\VC;%path%;
set path=%ProgramFiles(x86)%\Microsoft Visual Studio 10.0\VC;%path%;
set path=c:\python26;%path%;

call vcvarsall.bat
echo on
set include=c:\python26\include;%include%
set lib=c:\python26\libs;%lib%


rem csc /? && pause
rem cl /? & pause
rem swig --help

swig -python example.i
cl  example.c example_wrap.c /Fe"example.dll" /LD
move example.dll _example.pyd
python runme.py

pause

del *.dll
del *.obj
del *.exp
del *.lib
del *_wrap.c
del *PINVOKE.cs
del example.cs
del *.pyc
del example.py
del *.pyd

dir

pause
