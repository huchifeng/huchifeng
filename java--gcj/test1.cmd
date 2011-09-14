set TOP=D:\huchifeng.tmp\thisiscool-gcc\gcc-3.4
set path=%TOP%\bin;%path%
set GCC_PREFIX=i686-pc-mingw32-
set JNITEST_SO_NAME=sampNat.dll
set TOP=`cd ../gcc-3.4 && pwd`
set SWT_BASE_DIR=$TOP/../swt/win32
set SWT_GCJ_LIB_DIR=$SWT_BASE_DIR/lib
set SWT_JAVA_LIB_DIR=$SWT_BASE_DIR/swt-3038
set FILE_SEPARATOR=\;
set GCC_HOST=WIN32_HOST
set GCC_GCJH=gcjh
gcj HelloWorld.java --main=HelloWorld
dir
a.exe
pause
del a.exe
del *.bak
exit /B

大小写区分，不能写 gcj helloworld.java

