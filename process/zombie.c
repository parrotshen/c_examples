#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


#define MAKE_A_ZOMBIE (1)


int main(void)
{
    pid_t pid;

    printf("To check the zombie process:\n");
    printf("\n");
    printf("ps -C zombie -o ppid,pid,stat,cmd\n");
    printf("\n");
    printf("\n");

    pid = fork();

    if (pid < 0)
    {
        printf("Error ocurred !\n");
    }
    else
    {
        if (pid == 0)
        {
            printf("This is child process with pid of %d\n", getpid());
        }
        else
        {
            #if !(MAKE_A_ZOMBIE)
            wait(NULL);
            #endif

            sleep(20);

            printf("This is partent with pid of %d\n", getpid());
        }
    }

    exit(0);
}

