#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 10

void* createFileThread(void* input);
void cleanUp(void* input);

int main(void) {
	pthread_t threads[THREADS];
	int threadIds[THREADS];

	for(int i = 0; i < THREADS; i++) {
		threadIds[i] = i;
		pthread_create(&threads[i], NULL, createFileThread, &threadIds[i]);
	}

	sleep(1);

	srand(time(NULL));

	for(int i = 0; i < THREADS; i++) {
		if(rand() % 2) {
			if(pthread_cancel(threads[i]) != 0) {
				perror("Error cancelling thread\n");
				exit(EXIT_FAILURE);
			}

			printf("Thread %d cancelled\n", i);

		} else {
			printf("Thread %d not cancelled\n", i);
		}
	}

	for(int i = 0; i < THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return EXIT_SUCCESS;
}

void* createFileThread(void* input) {
	int* threadNum = (int*)input;
	char path[50];
	sprintf(path, "Thread%d.txt", *threadNum);

	FILE* fp = fopen(path, "w");

	if(fp == NULL) {
		perror("Error creating File!\n");
		exit(EXIT_FAILURE);
	}

	pthread_cleanup_push(cleanUp, input);
	while(sleep(2) > 0) {
	};
	pthread_cleanup_pop(0);

	pthread_exit(NULL);
}

void cleanUp(void* input) {
	int* threadNum = (int*)input;
	char path[50];

	sprintf(path, "Thread%d.txt", *threadNum);

	if(remove(path) != 0) {
		perror("Error removing file!\n");
		exit(EXIT_FAILURE);
	}
}