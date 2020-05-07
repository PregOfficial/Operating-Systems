#include "myqueue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS 50

void* threadCode(void* input);

pthread_spinlock_t spinlock;
myqueue queue;

typedef struct thread_data {
	int sum;
	int i;
} thread_data_t;

int main(void) {
	pthread_t threads[THREADS];
	thread_data_t threadsData[THREADS];

	if(pthread_spin_init(&spinlock, 0) != 0) {
		perror("Error creating mutex!");
		return EXIT_FAILURE;
	}

	myqueue_init(&queue);

	for(int i = 0; i < THREADS; i++) {
		threadsData[i].sum = 0;
		threadsData[i].i = i;

		pthread_create(&threads[i], NULL, threadCode, &threadsData[i]);
	}

	for(int i = 0; i < 10000; i++) {
		pthread_spin_lock(&spinlock);
		myqueue_push(&queue, 1);
		pthread_spin_unlock(&spinlock);
	}

	for(int i = 0; i < THREADS; i++) {
		pthread_spin_lock(&spinlock);
		myqueue_push(&queue, 0);
		pthread_spin_unlock(&spinlock);
	}

	int sum = 0;

	for(int i = 0; i < THREADS; i++) {
		pthread_join(threads[i], NULL);
		sum += threadsData[i].sum;
	}

	printf("complete sum = %d\n", sum);

	pthread_spin_destroy(&spinlock);

	return EXIT_SUCCESS;
}

void* threadCode(void* input) {
	thread_data_t* data = (thread_data_t*)input;

	bool running = true;
	int sum = 0;

	while(running) {
		pthread_spin_lock(&spinlock);
		if(!myqueue_is_empty(&queue)) {
			int value = myqueue_pop(&queue);
			if(value == 0) {
				running = false;
			}
			sum += value;
		}
		pthread_spin_unlock(&spinlock);
	}
	data->sum = sum;
	printf("sum of Thread %d = %d\n", data->i, data->sum);

	pthread_exit(NULL);
}

/*
Time for Exercise2 Sheet06
User time (seconds): 0.58
System time (seconds): 0.20

Time for this Excercise
User time (seconds): 0.58
System time (seconds): 0.20
*/