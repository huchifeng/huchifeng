rd /s /q db
mkdir db
set GIT_DIR=%~dp0db
git init --bare

call :add-file %0
call :add-file all.txt

type all.txt

pause
exit /b

:add-file
type %1
git hash-object -w %1 >> all.txt
echo = >> all.txt
echo %1 >> all.txt
exit /b


将文件直接存入 bare git 库，
另外写一个记录文件 all.txt
all.txt 也存入 git 库
恢复文件时根据 all.txt 取出即可。

需要做一个界面。
all.txt 设计为日志的作用。包括每个存入的文件，以及提交记录。
如果文件路径和hash都重复，不记录重复提交。
删除文件另外处理，直接删除 库中的 obj 即可？git fsck 也不会报告错误。

优点：自动根据HASH值排除重复文件，反复备份不占用空间。


git fsck -v --full
  列出 blob







