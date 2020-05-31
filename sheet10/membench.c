#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* threadCode();
void randomAllocate(void* allocations[], int allocationCounter);
void freeRandom(void* allocations[], int size);
void freeAll(void* allocations[], int size);

void (*custom_free)();
void* (*custom_malloc)(int);

int iterations;
int size;

/*
 * argv[1]: shared library
 * argv[2]: number of threads
 * argv[3]: number of iterations
 * argv[4]: size of memory segment
 */
int main(int argc, char* argv[]) {
	if(argc != 5) {
		printf("usage: %s <shared lib> <threads> <iterations> <size>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char* path = argv[1];
	int num_threads = atoi(argv[2]);
	iterations = atoi(argv[3]);
	size = atoi(argv[4]);

	// open library when the program needs the function
	void* lib = dlopen(path, RTLD_LAZY);

	if(lib == NULL) {
		perror("Error opening Library");
		return EXIT_FAILURE;
	}

	custom_malloc = dlsym(lib, "my_malloc");
	custom_free = dlsym(lib, "my_free");

	if(custom_malloc == NULL || custom_free == NULL) {
		perror("malloc or free function not found!");
		return EXIT_FAILURE;
	}

	void* nAllocations[iterations];
	void* n2Allocations[iterations / 2];
	randomAllocate(nAllocations, iterations);

	freeRandom(nAllocations, iterations);

	randomAllocate(n2Allocations, iterations / 2);
	freeAll(nAllocations, iterations);
	freeAll(n2Allocations, iterations / 2);

	pthread_t threads[num_threads];

	for(int i = 0; i < num_threads; i++) {
		pthread_create(&threads[i], NULL, threadCode, NULL);
	}

	for(int i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	dlclose(lib);

	return EXIT_SUCCESS;
}

void randomAllocate(void* allocations[], int allocationCounter) {
	for(int i = 0; i < allocationCounter; i++) {
		unsigned int seed = time(NULL);
		int random = rand_r(&seed) % 8;
		allocations[i] = custom_malloc(size + random * size);
	}
}

void freeRandom(void* allocations[], int size) {
	for(int i = 0; i < size; i++) {
		unsigned int seed = time(NULL);
		if(rand_r(&seed) % 2) {
			custom_free(allocations[i]);
			allocations[i] = NULL;
		}
	}
}

void freeAll(void* allocations[], int size) {
	for(int i = 0; i < size; i++) {
		if(allocations[i] != NULL) {
			custom_free(allocations[i]);
		}
	}
}

void* threadCode() {
	for(int i = 0; i < iterations; i++) {
		void* ptr = custom_malloc(size);
		custom_free(ptr);
	}

	pthread_exit(NULL);
}