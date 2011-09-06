import time;

def ack(x,y):
  if x == 0: return y + 1
  elif y == 0: return ack(x - 1, 1)
  else: return ack(x - 1, ack(x, y - 1))
for i in xrange(1,20):
  start = time.time();
  ack(3,6);
  print time.time() - start
