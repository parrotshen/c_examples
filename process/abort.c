#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 


void routine(int n)
{
    printf("[abort] SIGABRT\n"); 
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        printf("[abort] register signal handler\n");
        signal(SIGABRT, routine);
    }

    printf("[abort] before\n");

    abort();

    printf("[abort] after\n");

    return 0;
}

