#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WORKERS 20

int CREATE_DOOR = 1;
int CREATE_KNOB = 0;

typedef struct worker {
	int create_door;
} worker_t;

static void* worker_code(void* input);

static worker_t* find_worker(worker_t workers[], int type);