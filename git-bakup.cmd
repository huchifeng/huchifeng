set path=%ProgramFiles%\TortoiseGit\bin;%path%
set path=%ProgramFiles(x86)%\Git\bin;%path%;

cd /d %~dp0 || goto ::ERR

tortoiseproc /command:commit /path:"." /logmsg:"1"

set dst=..\..\huchifeng.3\1-02-github.zip
git.exe archive --format=zip --verbose HEAD > %dst% || goto :ERR

start %dst%
pause
exit /b

:ERR
pause



