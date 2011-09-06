"""
  cd /d %~dp0
  set path=c:\python32;%path%
  python %0
  pause
  exit /b

Tasklets — Lightweight threads

"""

import stackless

def func(*args, **kwargs):
    print("scheduled with", args, "and", kwargs)
    print(id(stackless.current))
t=stackless.tasklet(func)(1, 2, 3, string="test")
print(stackless.main == stackless.current); # true
print(stackless.current.is_main) # true
print(stackless.runcount) # 2
t2=stackless.tasklet(func)(12, 22, 32, string="test")
t.run() 
# t 执行后 t2 也会执行
# 如果 t2.run 则 t 不会执行，因为它在 chain 的前面

