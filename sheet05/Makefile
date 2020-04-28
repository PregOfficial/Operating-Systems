CFLAGS = -std=gnu11 -Wall -Werror -Wextra
LDLIBS=-lrt -lpthread

.PHONY: all clean

all: task1 task2

task1: task1_a task1_b
task2: task2_a task2_b

clean:
	$(RM) task1_a task1_b task2_a task2_b

task1_a: task1_a.c task1.h
task1_b: task1_b.c task1.h

task2_a: task2_a.c task1.h
task2_b: task2_b.c task1.h