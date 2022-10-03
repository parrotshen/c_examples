#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <error.h>

int main(void)
{
    struct rusage usage;
    int who = RUSAGE_SELF;

    if (getrusage(who, &usage) < 0)
    {
        perror( "getrusage" );
        return -1;
    }

    printf("\n");
    printf("CPU time (user)   %ld.%ld\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("CPU time (system) %ld.%ld\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
    printf("Maximum resident set size %ld\n", usage.ru_maxrss);
    printf("Soft page faults  %ld\n", usage.ru_minflt);
    printf("Hard page faults  %ld\n", usage.ru_majflt);
    printf("Filesystem input  %ld\n", usage.ru_inblock);
    printf("Filesystem output %ld\n", usage.ru_oublock);
    printf("Context switch (wait)  %ld\n", usage.ru_nvcsw);
    printf("Context switch (sched) %ld\n", usage.ru_nivcsw);
    printf("\n");

    return 0;
}

