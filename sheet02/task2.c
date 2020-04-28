#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

static void child_termination_handler();

int main(void)
{
    signal(SIGCHLD, child_termination_handler);
    pid_t pid;
    pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Fork failed");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        printf("Child %d sleeping for 5 seconds\n", getpid());
        sleep(5);
        printf("Child done\n");
        exit(0);
    }
    else
    {
        pause();
    }
    return EXIT_SUCCESS;
}

static void child_termination_handler()
{
    printf("Parent done\n");
}