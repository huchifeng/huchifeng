# nmake nmake1.mak
AAA=BBB

#  %if $(AAA) == BBB
#  echo AAA!!
#  %endif
#  可能不支持 %if

# % abort fail-message
# 不支持

all: # 行注释 
  echo hello,world; 不用TAB开头也可以
# 注释必须没有空白缩进
  echo 没有块注释 # 这里仍然被echo接收
  echo 1 #注释\
续行

test: test2 \
  test3 # 依赖关系可以续行，续行前不能注释
# test2 先执行
  echo test

test2:
  echo test2

test3: nmake1.cmd
  echo test3

test2 test3 : test4 # 例如多个.cpp依赖一个.h
#  test4 只需要执行一次

test4: 
  echo test4 $(AAA) $(BBB)
# 未定义变量是空
  echo $(PATH)
# 自动导入环境变量为宏
  echo $(CCC)
# 来自命令行的宏
# echo $(.TARGET)
#    不支持 $(.TARGET) $(.SOURCE) ？
# echo  $(AAA,B=bbb)
#    不支持替换，例如 $(OBJS,.obj=.c) 应该得到 .c 列表；
#    $(name,modifier[,modifier ...]) 很多特殊 modifier
#    $(SRCS,D) 将 d:\src\main.c io.asm 替换为 d:\src .
#    $(SRCS,E) 将 d:\src\main.c io.asm 替换为 .c .asm
#    $(SRCS,F) 将 d:\src\main.c io.asm 替换为 main.c io.asm
# exit /b 1 
#    导致make失败
  - exit /b 1
#    忽略返回值，不失败

%.cmd : %.h
  echo match any
# 应该是预定义 
# %.obj : %.c
#	$(CC) $(CFLAGS) –c $(.SOURCE) 
# 或者
# .c.obj :
#	$(CC) $(CFLAGS) –c $(.SOURCE) 

%.h :
  echo any .h
