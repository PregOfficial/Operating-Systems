#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define WRITE 1
#define READ 0

int main(int argc, char* argv[]) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <keyword>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int p[2];

	if(pipe(p) == -1) {
		fprintf(stderr, "Pipe failed");
		exit(EXIT_FAILURE);
	}

	pid_t pid;
	pid = fork();

	if(pid < 0) {
		fprintf(stderr, "Fork failed");
		exit(EXIT_FAILURE);
	} else if(pid == 0) {
		dup2(p[READ], STDIN_FILENO);
		execl("/bin/grep", "grep", argv[1], NULL);
		exit(0);
	} else {
		// PARENT
		close(p[READ]);
		dup2(p[WRITE], STDOUT_FILENO);
		close(p[READ]);

		char* args[2];
		args[0] = "/bin/ls";
		args[1] = NULL;

		execv(args[0], args);

		wait(NULL);
	}

	return EXIT_SUCCESS;
}