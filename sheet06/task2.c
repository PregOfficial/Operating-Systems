#include "myqueue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS 5

void* threadCode(void* input);

pthread_mutex_t mutex;

typedef struct thread_data {
	myqueue* queue;
	int sum;
	int i;
} thread_data_t;

int main(void) {

	myqueue queue;
	pthread_t threads[THREADS];
	thread_data_t threadsData[THREADS];

	if(pthread_mutex_init(&mutex, NULL) != 0) {
		perror("Error creating mutex!");
		return EXIT_FAILURE;
	}

	pthread_mutex_lock(&mutex);
	myqueue_init(&queue);

	for(int i = 0; i < THREADS; i++) {
		threadsData[i].queue = &queue;
		threadsData[i].sum = 0;
		threadsData[i].i = i;

		pthread_create(&threads[i], NULL, threadCode, &threadsData[i]);
	}

	for(int i = 0; i < 10000; i++) {
		myqueue_push(&queue, 1);
	}

	for(int i = 0; i < 5; i++) {
		myqueue_push(&queue, 0);
	}

	pthread_mutex_unlock(&mutex);

	int sum = 0;

	for(int i = 0; i < THREADS; i++) {
		pthread_join(threads[i], NULL);
		sum += threadsData[i].sum;
	}

	printf("complete sum = %d\n", sum);

	pthread_mutex_destroy(&mutex);

	return EXIT_SUCCESS;
}

void* threadCode(void* input) {
	thread_data_t* data = (thread_data_t*)input;

	bool running = true;
	int sum = 0;

	while(running) {
		pthread_mutex_lock(&mutex);
		if(!myqueue_is_empty(data->queue)) {
			int value = myqueue_pop(data->queue);
			if(value == 0) {
				running = false;
			}
			sum += value;
		}
		pthread_mutex_unlock(&mutex);
	}
	data->sum = sum;
	printf("sum of Thread %d = %d\n", data->i, data->sum);

	pthread_exit(NULL);
}