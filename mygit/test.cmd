mygit || goto :err
mygit xxx && goto :err

del 1.db || goto :err
echo xx >> 1.db || goto :err
mygit dir --db 1.db && goto :err

del 1.db || goto :err

mygit add --db 1.db --type a --value abcdef || goto :err
ls -l 1.db || goto :err
mygit add --db 1.db --type a --value abcdef || goto :err
ls -l 1.db || goto :err
mygit add --db 1.db --type a --value abcdef-hijklmn || goto :err
ls -l 1.db || goto :err
od -t x1z -N 100 1.db || goto :err

mygit add --db 1.db --type a --file stdafx.h || goto :err
ls -l stdafx.h || goto :err
ls -l 1.db || goto :err
od -t x1z -N 200 1.db || goto :err

mygit dir --db 1.db || goto :err

mygit get --db 1.db --id 1 || goto :err
mygit get --db 1.db --id 2 || goto :err
mygit get --db 1.db --id 1 > tmp.1 || goto :err
ls -l tmp.1 || goto :err

mygit get --db 1.db --type a || goto :err

mygit get --db 1.db --id 3 --file-out tmp.2 || goto :err
ls -l tmp.2 stdafx.h || goto :err
fc /b tmp.2 stdafx.h || goto :err

mygit del --db 1.db --id 2 || goto :err
ls -l 1.db || goto :err
mygit dir --db 1.db || goto :err

mygit del --db 1.db --id 2 && goto :err
mygit del --db 1.db --id 20 && goto :err
mygit undel --db 1.db --id 2 || goto :err
mygit dir --db 1.db || goto :err
mygit del --db 1.db --id -2 && goto :err
mygit undel --db 1.db --id -2 && goto :err

mygit del --db 1.db --id 2 || goto :err
mygit pack --db 1.db || goto :err
ls -l 1.db || goto :err
mygit dir --db 1.db || goto :err
mygit add --db 1.db --type a --value zyx || goto :err
mygit add --db 1.db --type a --file test.jpg || goto :err
mygit del --db 1.db --id 1 || goto :err
mygit del --db 1.db --id 3 || goto :err
mygit pack --db 1.db || goto :err
ls -l 1.db || goto :err
mygit dir --db 1.db || goto :err
mygit del --db 1.db --id 4 || goto :err

mygit get --db 1.db --id 5 --file-out tmp.jpg || goto :err
fc /b tmp.jpg test.jpg || goto :err
mygit del --db 1.db --id 5 || goto :err

mygit pack --db 1.db || goto :err
ls -l 1.db || goto :err
mygit dir --db 1.db || goto :err

mygit hash --file HelloWorld-colliding.exe || goto :err
mygit hash --value abcdef || goto :err

del tmp.* || goto :err

exit /b

:err
echo error:%errorlevel%

exit /b

