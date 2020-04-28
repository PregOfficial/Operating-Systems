#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

static void handle_sigusr1();
static void handle_sigusr2();
static void handle_sigalrm();
static void handle_sigalrm_child();
static void handle_child_termination();

int main(void)
{
    sigset_t set;
    sigset_t oldSet;
    sigaddset(&set, SIGUSR2);

    signal(SIGCHLD, handle_child_termination);
    signal(SIGALRM, handle_sigalrm);
    pid_t pid;
    pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Fork failed");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        sigprocmask(SIG_BLOCK, &set, &oldSet);
        signal(SIGUSR1, handle_sigusr1);
        signal(SIGUSR2, handle_sigusr2);
        signal(SIGALRM, handle_sigalrm_child);

        alarm(13);

        while (1)
            ;

        return 0;
    }
    else
    {
        alarm(3);
        pause();
        kill(pid, SIGUSR1);
        kill(pid, SIGUSR2);
        while (1)
        {
            alarm(3);
            pause();
            kill(pid, SIGUSR1);
        }
    }
    return EXIT_SUCCESS;
}

static void handle_sigusr1()
{
    fprintf(stdout, "Received SIGUSR1\n");
}

static void handle_sigusr2()
{
    fprintf(stdout, "Child done\n");
    exit(0);
}

static void handle_sigalrm()
{
}

static void handle_sigalrm_child()
{
    sigset_t set;
    sigset_t oldSet;
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_UNBLOCK, &set, &oldSet);
}

static void handle_child_termination()
{
    fprintf(stdout, "Parent done\n");
    exit(0);
}