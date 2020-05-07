#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS 1000
#define ITERATIONS 10000

void* threadCode(void* input);

pthread_mutex_t mutex;

int main(void) {
	int x = 0;
	pthread_t threads[THREADS];

	if(pthread_mutex_init(&mutex, NULL) != 0) {
		perror("error creating mutex!");
		return EXIT_FAILURE;
	}

	for(int i = 0; i < THREADS; i++) {
		pthread_create(&threads[i], NULL, threadCode, &x);
	}

	for(int i = 0; i < THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Sum = %d\n", x);

	return EXIT_SUCCESS;
}

void* threadCode(void* input) {
	int* x = (int*)input;

	for(int i = 0; i < ITERATIONS; i++) {
		pthread_mutex_lock(&mutex);
		(*x)++;
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}

/*
/usr/bin/time -v ./task3b

User time (seconds): 0.58
System time (seconds): 0.20

*/