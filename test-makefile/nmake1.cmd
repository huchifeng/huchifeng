
call "C:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"

echo on

nmake /f nmake1.mak

nmake /f nmake1.mak test "CCC=C C C"

pause

