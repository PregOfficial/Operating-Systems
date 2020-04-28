#include "task1.h"
#include <semaphore.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
	int shm_fd = shm_open(shmName, O_RDWR, 0666);

	sharedstr_t* shmstr =
	    (sharedstr_t*)mmap(0, sizeof(sharedstr_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	pid_t producerPid, consumerPid;

	sem_t* consSem = sem_open("/sem1", O_CREAT | O_EXCL, 0666, 0);
	sem_t* prodSem = sem_open("/sem2", O_CREAT | O_EXCL, 0666, 9);

	producerPid = fork();
	consumerPid = fork();

	if(producerPid < 0 || consumerPid < 0) {
		printf("Fork failed!\n");
		return EXIT_FAILURE;
	} else if(producerPid == 0) {
		for(uint64_t i = 0; i < shmstr->n; i++) {
			sem_wait(prodSem);
			shmstr->ringBuffer[i % 10] = i + 1;
			sem_post(consSem);
		}
		exit(0);
	} else if(consumerPid == 0) {
		uint64_t temp = 0;
		for(uint64_t i = 0; i < shmstr->n; i++) {
			sem_wait(consSem);
			temp += shmstr->ringBuffer[i % 10];
			sem_post(prodSem);
		}
		shmstr->result = temp;
		exit(0);
	}

	while(wait(0) > 0) {
	}

	int fd = open(fifoPath, O_WRONLY);
	char result[BUFFER_SIZE];
	sprintf(result, "%ld\n", shmstr->result);

	if(write(fd, result, sizeof(result)) == -1) {
		printf("Error writing to fifo\n");
		return EXIT_FAILURE;
	}

	if(sem_unlink("/sem1") == -1 || sem_unlink("/sem2") == -1 || sem_destroy(consSem) == -1 ||
	   sem_destroy(prodSem) == -1) {
		printf("Error destroying Semaphores!\n");
		return EXIT_FAILURE;
	}

	return 0;
}

/*
N = 1000
/usr/bin/time -p ./task2_b
result: 0.18
*/