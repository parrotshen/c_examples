#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 


void routine(int n)
{
    printf("[pause] SIGINT\n"); 
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        printf("[pause] register signal handler\n");
        signal(SIGINT, routine);
    }

    printf("[pause] waiting for 'CTRL-C' ...\n\n");

    pause();

    printf("[pause] terminated\n");

    return 0;
}

