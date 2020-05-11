#include "myqueue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 50

typedef struct thread_data {
	int sum;
	int i;
} thread_data_t;

void* threadCode(void* input);

pthread_mutex_t mutex;
pthread_cond_t cond;

myqueue queue;

int main(void) {
	pthread_t threads[THREADS];
	thread_data_t threadsData[THREADS];

	if(pthread_mutex_init(&mutex, NULL) != 0) {
		perror("Error creating mutex!");
		return EXIT_FAILURE;
	}

	if(pthread_cond_init(&cond, NULL) != 0) {
		perror("Error creating condition variable!");
		return EXIT_FAILURE;
	}

	myqueue_init(&queue);

	for(int i = 0; i < THREADS; i++) {
		threadsData[i].sum = 0;
		threadsData[i].i = i;

		pthread_create(&threads[i], NULL, threadCode, &threadsData[i]);
	}

	for(int i = 0; i < (10000 + THREADS); i++) {
		pthread_mutex_lock(&mutex);
		if(i < 10000) {
			myqueue_push(&queue, 1);
		} else {
			myqueue_push(&queue, 0);
		}
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);
	}

	int sum = 0;

	for(int i = 0; i < THREADS; i++) {
		pthread_join(threads[i], NULL);
		sum += threadsData[i].sum;
	}

	printf("complete sum = %d\n", sum);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return EXIT_SUCCESS;
}

void* threadCode(void* input) {
	thread_data_t* data = (thread_data_t*)input;

	bool running = true;
	int sum = 0;

	while(running) {
		pthread_mutex_lock(&mutex);
		if(!myqueue_is_empty(&queue)) {
			int value = myqueue_pop(&queue);
			if(value == 0) {
				running = false;
			}
			sum += value;
		} else {
			pthread_cond_wait(&cond, &mutex);
		}
		pthread_mutex_unlock(&mutex);
	}
	data->sum = sum;
	printf("sum of Thread %d = %d\n", data->i, data->sum);

	pthread_exit(NULL);
}

/*
User time (seconds): 0.00
System time (seconds): 0.00
*/