set path=%windir%\Microsoft.NET\Framework\v2.0.50727\;%path%
jsc.exe /?
jsc.exe /fast- test.js
test.exe
del test.exe

pause