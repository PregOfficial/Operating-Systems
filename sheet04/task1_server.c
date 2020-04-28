#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 60

int main(int argc, char** argv) {
	if(argc <= 1) return EXIT_FAILURE;

	int fdFifos[argc - 1];
	int maxFd = 0;

	for(int i = 1; i < argc; i++) {
		if(mkfifo(argv[i], 0666) == -1) {
			perror("error creating fifo");
			exit(EXIT_FAILURE);
		}
	}

	fd_set fds;

	int unused __attribute__((unused));
	char buffer[BUFFER_SIZE];
	int connections = 0;
	int disconnected = 0;

	while(1) {
		if(connections < argc - 1) {
			fdFifos[connections] = open(argv[connections + 1], O_RDONLY);
			if(fdFifos[connections] > maxFd) maxFd = fdFifos[connections];
			printf("%s: connected\n\n", argv[connections + 1]);
		}

		FD_ZERO(&fds);

		for(int i = 0; i < argc - 1; i++) {
			FD_SET(fdFifos[i], &fds);
		}

		if(select(maxFd + 1, &fds, NULL, NULL, NULL) < 0) perror("select()");

		for(int i = 0; i < argc - 1; i++) {
			memset(buffer, 0, BUFFER_SIZE * sizeof(char));
			if(FD_ISSET(fdFifos[i], &fds)) {
				unused = read(fdFifos[i], buffer, sizeof(buffer));
				if(buffer[0] == '\n') {
					disconnected++;
					fprintf(stdout, "%s: disconnected\n", argv[i + 1]);
					close(fdFifos[i]);
					fdFifos[i] = 0;

					// if all clients are disconnected
					if(disconnected == argc - 1) {
						// delete pipes
						for(int i = 1; i < argc; i++) {
							unlink(argv[i]);
						}
						exit(EXIT_SUCCESS);
					}
				} else {
					fprintf(stdout, "%s: %s\n", argv[i + 1], buffer);
				}
			}
		}
		if(connections < argc - 1) connections++;
	}
	return EXIT_SUCCESS;
}