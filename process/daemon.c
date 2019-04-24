#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void sig_term(int signo)
{
    /* Catched signal sent by kill(1) command */
    if (SIGTERM == signo)
    {
        printf("[daemon] program terminated\n");
        exit(0);
    }
}

int main(void)
{
    pid_t pid = fork();

    if (pid > 0)
    {
        printf("[daemon] this is the parent process\n");
        printf("[daemon] create a daemon PID=%d\n\n", pid);
        return 0;
    }
    else if (pid < 0)
    {
        printf("[daemon] this is the parent process\n");
        printf("[daemon] fail to fork daemon\n\n");
        return pid;
    }
    else
    {
        printf("[daemon] this is the child process\n");

        signal(SIGTERM, sig_term);

        while (1)
        {
            time_t t;

            t = time(NULL);
            printf( ctime( &t ) );
            sleep(5);
        }
    }

    return 0;
}

