set path=%windir%\Microsoft.NET\Framework\v4.0.30319;%path%
csc  /out:test.exe basic.cs
test
pause
