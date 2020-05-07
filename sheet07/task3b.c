#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS 1000
#define ITERATIONS 10000

void* threadCode(void* input);

int main(void) {
	_Atomic(int) x = 0;
	pthread_t threads[THREADS];

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
	_Atomic(int)* x = (_Atomic(int)*)input;

	for(int i = 0; i < ITERATIONS; i++) {
		(*x)++;
	}

	pthread_exit(NULL);
}

/*
/usr/bin/time -v ./task3b

User time (seconds): 0.24
System time (seconds): 0.04

*/