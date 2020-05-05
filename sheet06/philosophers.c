#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EAT_DURATION 500
#define NUM_PHILOSOPHER 5
#define EAT_ITERATIONS 1000
#define RIGHT_CHOPSTICK(n) (n)
#define LEFT_CHOPSTICK(n) (((n) + 1) % NUM_PHILOSOPHER)

pthread_t philosopher[NUM_PHILOSOPHER];
pthread_mutex_t chopstick[NUM_PHILOSOPHER];

void* dine(void* id) {
	int n = (int)(intptr_t)id;
	for(int i = 0; i < EAT_ITERATIONS; ++i) {
		pthread_mutex_lock(&chopstick[RIGHT_CHOPSTICK(n)]);
		if(pthread_mutex_trylock(&chopstick[LEFT_CHOPSTICK(n)]) != 0) {
			pthread_mutex_unlock(&chopstick[RIGHT_CHOPSTICK(n)]);
		} else {
			usleep(EAT_DURATION);
			pthread_mutex_unlock(&chopstick[LEFT_CHOPSTICK(n)]);
			pthread_mutex_unlock(&chopstick[RIGHT_CHOPSTICK(n)]);
		}
	}
	printf("Philosopher %d is done eating!\n", n);

	return NULL;
}

int main() {
	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_mutex_init(&chopstick[i], NULL);
	}

	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_create(&philosopher[i], NULL, dine, (void*)(intptr_t)i);
	}

	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_join(philosopher[i], NULL);
	}

	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_mutex_destroy(&chopstick[i]);
	}

	return EXIT_SUCCESS;
}

/*
Explain the program:
there are the same amount of forks as philosophers and every philosopher
needs 2 forks to eat. So not all philosophers are able to eat at the same time,
in this program it's realised with mutexs

How can a deadlock occur? Does it always happen?
A deadlock occurs when a philosopher can pick up his right fork, but not
his left one. In this example nearly everytime it happens, because it's
very unlikely that the threads pick up the forks in the right order.

Change the program to prevent any potential deadlocks from occurring. Explain how your changes
prevent deadlocks.
I added in the thread a mutex trylock on the left fork, so when it's not possible
to lock the mutex then the right one unlocks.
*/