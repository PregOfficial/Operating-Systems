#include "task1.h"
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
	int shm_fd = shm_open(shmName, O_RDWR, 0666);

	sharedstr_t* shmstr =
	    (sharedstr_t*)mmap(0, sizeof(sharedstr_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	pid_t producerPid, consumerPid;
	producerPid = fork();
	consumerPid = fork();

	if(producerPid < 0 || consumerPid < 0) {
		printf("Fork failed!\n");
		return EXIT_FAILURE;
	} else if(producerPid == 0) {
		for(int i = 0; i < shmstr->n; i++) {
			shmstr->ringBuffer[i % 10] = i + 1;
		}
		exit(0);
	} else if(consumerPid == 0) {
		uint64_t temp = 0;
		for(int i = 0; i < shmstr->n; i++) {
			temp += shmstr->ringBuffer[i % 10];
		}
		shmstr->result = temp;
		exit(0);
	}

	waitpid(producerPid, NULL, 0);
	waitpid(consumerPid, NULL, 0);

	int fd = open(fifoPath, O_WRONLY);
	char result[10];
	sprintf(result, "%ld\n", shmstr->result);

	if(write(fd, result, sizeof(result)) == -1) {
		printf("Error writing to fifo\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}