#include "task1.h"

#define BUFFER_SIZE 100

int main(int arc, char* argv[]) {
	if(arc != 2) {
		printf("Usage: %s N\n", argv[0]);
		return EXIT_FAILURE;
	}

	char* p;
	int shm_fd = shm_open(shmName, O_CREAT | O_RDWR, 0666);
	uint64_t n = strtol(argv[1], &p, 10);

	if(ftruncate(shm_fd, sizeof(sharedstr_t)) == -1) {
		printf("Error\n");
		return EXIT_FAILURE;
	}

	sharedstr_t* ptr =
	    (sharedstr_t*)mmap(0, sizeof(sharedstr_t), PROT_WRITE, MAP_SHARED, shm_fd, 0);
	ptr->n = n;

	if(mkfifo(fifoPath, 0666) == -1) {
		printf("Error creating FIFO\n");
		return EXIT_FAILURE;
	}

	char buffer[BUFFER_SIZE];
	int fifoFd = open(fifoPath, O_RDONLY);
	read(fifoFd, buffer, BUFFER_SIZE);

	printf("Result: %s\n", buffer);

	if(unlink(fifoPath) == -1) {
		printf("Error unlinking fifo\n");
		return EXIT_FAILURE;
	}
	if(shm_unlink(shmName) == -1) {
		printf("Error unlinking shared memory\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
N = 1000
/usr/bin/time -p ./task2_b
result: 0.0
*/