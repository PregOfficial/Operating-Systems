#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct train {
	unsigned int number;
	unsigned int arrivalTime;
	unsigned int stopTime;
} train_t;

typedef struct station {
	unsigned int number;
	train_t* train;
} station_t;

void* train_code(void* input);

static void sigint_handler();
