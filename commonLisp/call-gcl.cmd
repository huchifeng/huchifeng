
path C:\_install\GCL-2.6.6-ANSI\mingw\bin;C:\_install\GCL-2.6.6-ANSI\lib\gcl-2.6.6\unixport;%PATH%
C:\_install\GCL-2.6.6-ANSI\lib\gcl-2.6.6\unixport\saved_ansi_gcl.exe -dir C:/_install/GCL-2.6.6-ANSI/lib/gcl-2.6.6/unixport/ -libdir C:/_install/GCL-2.6.6-ANSI/lib/gcl-2.6.6/ -f  2.lisp

pause
exit /b
交互
环境，
输入 (f) 出错进入dbg
  按 1 回到 top level
  dbg下其他表达式
都可以输入
    继续出错进入 level 2；可选 1回到level-1， 2回到top
      再错则 crash

gcl-si/Command-Line.html
  -f 执行文件，此时可以用 read-line *query-io*

