CFLAGS = -std=gnu11 -Wall -Werror -Wextra
LDLIBS=-lrt -pthread

.PHONY: all clean

all: task1 task2 philosophers

task1: task1.c
task2: task2.c myqueue.h
philosophers: philosophers.c

clean:
	$(RM) task1 Thread*.txt task2 philosophers