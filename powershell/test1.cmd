# || goto :CMD

$a=0;
$c=0;
dir ..\ -recurse -filter *.cpp | foreach{ $a++; $c += (type $_.FullName).Length }
[datetime]::Now
""+$a+","+$c

exit


:CMD
cd /D %~dp0
copy %0 %~dpn0.ps1
powershell set-ExecutionPolicy remotesigned
powershell %~dpn0.ps1
del %~dpn0.ps1
pause
exit /B

powershell set-ExecutionPolicy restricted
必须设置策略，然后才能执行脚本



