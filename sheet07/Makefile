CFLAGS = -std=gnu11 -Wall -Werror -Wextra
LDLIBS=-lrt -pthread

.PHONY: all clean

all: task1 task2 task3

task1: task1.c myqueue.h
task2: task2.c myqueue.h

task3: task3a task3b

task3a: task3a.c
task3b: task3b.c

clean:
	$(RM) task1 task2 task3a task3b