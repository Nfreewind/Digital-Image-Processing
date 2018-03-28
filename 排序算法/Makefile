CC = g++
C_FLAGS = -g -Wall
INCLUDE =
LIBS =
#wildcard对cpp文件进行展开
SRCS := $(wildcard *.cpp)
#将SRCS中后缀.cpp替换成空，即可执行程序的名字与cpp名字相同
TARGET  := $(patsubst %.cpp, %, $(SRCS))
OBJDIR = .

.cpp:
	   $(CC) $(C_FLAGS) -o $@ $< $(INCLUDE) $(LIBS)
	      #可移动到指定的OBJDIR目录
		     #mv $@ $(OBJDIR)
all: $(TARGET) clean
	clean:
	  rm -rf *.o
