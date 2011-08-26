set path=c:\python26;%path%
set waf=C:\Users\hcf\Desktop\waf
cd %~dp0



python %waf%\waf hello

python %waf%\waf ping pong ping pong

pause


exit /b

waf 没有含义

samba, node.js 使用 waf

可以生成 for IDE 项目文件(eclipse, vs);

wget http://waf.googlecode.com/files/waf-1.6.6.tar.bz2
python waf --version
python waf-light

waf 文件中包括 python代码和二进制数据(library)。
可以只下载一个文件使用。

python %waf%\waf --version

rem python %~dp0test1.py
python waf-light --make-waf --tools=compat15,test1.py --prelude=$'\tfrom waflib.extras import test1\n\ttest1.foo()'
从源码编译waf用的？（构造一个 waf 文件，含脚本和library）
自定义一段脚本。

python %waf%\waf --help