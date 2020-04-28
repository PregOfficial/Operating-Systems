#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 50

int main(int argc, char** argv) {
	if(argc != 2) return EXIT_FAILURE;
	int pipe = open(argv[1], O_WRONLY);

	if(pipe == -1) return EXIT_FAILURE;

	char buffer[BUFFER_SIZE];
	int unused __attribute__((unused));

	while(*(fgets(buffer, BUFFER_SIZE, stdin)) != '\n') {
		char message[BUFFER_SIZE];
		sprintf(message, "%s", buffer);
		unused = write(pipe, message, strlen(message) + 1);
		memset(buffer, 0, BUFFER_SIZE * sizeof(char));
	}
	unused = write(pipe, "\n", sizeof(buffer));

	return EXIT_SUCCESS;
}