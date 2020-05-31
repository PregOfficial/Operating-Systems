#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* threadCode();

void (*custom_free)(void*);
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

void* threadCode() {
	for(int i = 0; i < iterations; i++) {
		void* ptr = custom_malloc(size);
		custom_free(ptr);
	}

	pthread_exit(NULL);
}