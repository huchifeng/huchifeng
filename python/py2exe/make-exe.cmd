set path=c:\python26\;%path%
cd /d %~dp0

python setup.py py2exe --help

python setup.py py2exe
move dist dist1
rem 一个 dist 目录下得到 exe, library.zip, *.pyd, pytho26.dll 等

python setup.py py2exe --bundle-files 1
rem 得到单个 exe 文件

pause
exit /B



python 2.6
  msvcr90.exe 9.0.21022.8
  Microsoft Visual C++ 2008 Redistributable Package published 29-11-2007, 
  so not the VS2008 SP1 one (tested with Python 2.7.1).

值为1表示pyd和dll文件会被打包到exe文件中，且不能从文件系统中加载python模块；
值为2表示pyd和dll文件会被打包到exe文件中，但是可以从文件系统中加载python模块。
另外setup中使用zipfile=None可以不生成library.zip。 


