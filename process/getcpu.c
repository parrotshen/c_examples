#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("pid[%u] on cpu[%d]\n", getpid(), sched_getcpu());
    if (argc > 1)
    {
        system("./getcpu");
    }
    return 0;
}
