#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <error.h>

int main(void)
{
    struct rlimit rlim;

    printf("\n");

    printf("RLIMIT_AS ................ ");
    if (getrlimit(RLIMIT_AS, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_CORE .............. ");
    if (getrlimit(RLIMIT_CORE, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_CPU ............... ");
    if (getrlimit(RLIMIT_CPU, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_DATA .............. ");
    if (getrlimit(RLIMIT_DATA, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_FSIZE ............. ");
    if (getrlimit(RLIMIT_FSIZE, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_LOCKS ............. ");
    if (getrlimit(RLIMIT_LOCKS, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_MEMLOCK ........... ");
    if (getrlimit(RLIMIT_MEMLOCK, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_MSGQUEUE .......... ");
    if (getrlimit(RLIMIT_MSGQUEUE, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_NICE .............. ");
    if (getrlimit(RLIMIT_NICE, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_NOFILE ............ ");
    if (getrlimit(RLIMIT_NOFILE, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_NPROC ............. ");
    if (getrlimit(RLIMIT_NPROC, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_RSS ............... ");
    if (getrlimit(RLIMIT_RSS, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_RTPRIO ............ ");
    if (getrlimit(RLIMIT_RTPRIO, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    //printf("RLIMIT_RTTIME ............ ");
    //if (getrlimit(RLIMIT_RTTIME, &rlim) < 0)
    //{
    //    printf("failed\n");
    //    perror( "getrlimit" );
    //}
    //printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_SIGPENDING ........ ");
    if (getrlimit(RLIMIT_SIGPENDING, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("RLIMIT_STACK ............. ");
    if (getrlimit(RLIMIT_STACK, &rlim) < 0)
    {
        printf("failed\n");
        perror( "getrlimit" );
    }
    printf("%lu / %lu\n", rlim.rlim_cur, rlim.rlim_max);

    printf("\n");

    return 0;
}

