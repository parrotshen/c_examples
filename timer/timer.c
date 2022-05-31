#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>


unsigned long long g_nsec = 1000000000;
int g_signo = SIGUSR1;
int g_oneshot = 0;
timer_t g_tid = 0;


void timeout(int arg)
{
    struct tm *nPtr;
    time_t t = 0;

    time( &t );
    nPtr = localtime( &t );

    if ( g_oneshot )
    {
        printf(
            "Timeout ... %02d:%02d:%02d (one-shot)\n",
            nPtr->tm_hour,
            nPtr->tm_min,
            nPtr->tm_sec
        );
        timer_delete( g_tid );
    }
    else
    {
        printf(
            "Timeout ... %02d:%02d:%02d\n",
            nPtr->tm_hour,
            nPtr->tm_min,
            nPtr->tm_sec
        );
    }
}

int main(int argc, char *argv[])
{
    struct sigevent sev;
    struct itimerspec its;


    if (argc > 1)
    {
        g_oneshot = atoi( argv[1] );
    }

    signal(g_signo, timeout);

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = g_signo;
    sev.sigev_value.sival_ptr = &g_tid;

    if (timer_create(CLOCK_REALTIME, &sev, &g_tid) != 0)
    {
        perror( "timer_create" );
        exit(-1);
    }

    its.it_value.tv_sec = (g_nsec / 1000000000);
    its.it_value.tv_nsec = (g_nsec % 1000000000);
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if (timer_settime(g_tid, 0, &its, NULL) != 0)
    {
        perror( "timer_settime" );
        return -1;
    }

    printf(
        "[Timer ID   ] %lu\n",
        (unsigned long)g_tid
    );
    printf(
        "[Timer value] %lu.%lu (sec)\n\n",
        its.it_interval.tv_sec,
        its.it_interval.tv_nsec
    );

    getchar();

    timer_delete( g_tid );

    exit(0);
}

