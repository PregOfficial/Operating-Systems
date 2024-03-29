#ifndef _TASK1_H_
#define _TASK1_H_

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
	uint64_t n;
	uint64_t ringBuffer[10];
	uint64_t result;
} sharedstr_t;

const char* fifoPath = "task_fifo";
const char* shmName = "task_shm";

#endif