#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

unsigned fib(unsigned n);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("You must provide a number of childprocesses\n");
        return EXIT_FAILURE;
    }
    unsigned int n = strtol(argv[1], NULL, 10);
    for (unsigned int i = 0; i < n; i++)
    {
        pid_t pid;
        pid = fork();

        if (pid < 0)
        {
            fprintf(stderr, "Fork failed");
            return EXIT_FAILURE;
        }
        else if (pid == 0)
        {
            printf("Child %d PID = %d. Fib(40) = %d\n", i, getpid(), fib(40));
            return 0;
        }
        else
        {
            wait(NULL);
        }
    }
    printf("Done\n");
    return EXIT_SUCCESS;
}

unsigned fib(unsigned n)
{
    if (n <= 1)
        return n;
    return fib(n - 1) + fib(n - 2);
}

/*
Analyze the obtained output. Is the order of the messages consistent? Can the order of these messages be predicted? What does it depend on?

Yes it is, because the program waits for the childprocess before starting a new one.
*/