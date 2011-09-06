"""
  cd /d %~dp0
  set path=c:\python32;%path%
  python %0
  pause
  exit /b


"""

import stackless

c = stackless.channel()
def sender(chan, value):
    chan.send(value)
    print("sender done")
t=stackless.tasklet(sender)(c, 5)
t.run() # 创造出第二个 tasklet，否则因无tasklet可接收，会block唯一tasklet
print("received:", c.receive())


def receiver(chan):
    print("i am waiting...")
    value = chan.receive()
    print("i got:", value)
stackless.tasklet(receiver)(c)
stackless.run()
print("now sending...")
c.send(500)
