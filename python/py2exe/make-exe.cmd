set path=c:\python26\;%path%
cd /d %~dp0

python setup.py py2exe
pause
exit /B

dist 目录下得到 exe

python 2.6
  msvcr90.exe 9.0.21022.8
  Microsoft Visual C++ 2008 Redistributable Package published 29-11-2007, 
  so not the VS2008 SP1 one (tested with Python 2.7.1).