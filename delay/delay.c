/* sleep / usleep / select / pthead */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


int main(int argc, char *argv[])
{ 
    unsigned int delay[20] = 
        { 500000, 100000, 50000, 10000, 1000, 900, 500, 100, 10, 1, 0 }; 
    unsigned int nTimeTest = 0; /* usec */ 
    unsigned int nDelay = 0; /* usec */ 
    int nReduce = 0;
    struct timeval tvBegin; 
    struct timeval tvNow; 

    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    struct timespec req;
    struct timeval tv; 
    fd_set rfds; 
    int fd = 1; 
    int ret; 
    int i; 


    #if 0
    if (argc < 2)
    { 
        fprintf(stderr, "Usage: delay USEC\n"); \
        fprintf(stderr, "\n"); \
        return 0; 
    } 
    nDelay = atoi (argv[1]); 
    #endif 

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    
    fprintf(
        stderr,
        "   function      time(usec)    realTime     reduce\n"
    );
    fprintf(
        stderr, 
        "-----------------------------------------------------\n"
    ); 

    for (i=0; i<20; i++)
    { 
        if (delay[i] <= 0) break;

        nDelay = delay[i]; 


        /* [1] test usleep */ 
        gettimeofday(&tvBegin, NULL); 
        ret = usleep( nDelay );
        if (-1 == ret) 
        { 
            fprintf(
                stderr,
                "   usleep        error        .           errno=%d [%s]\n",
                errno, 
                strerror(errno)
            );
        }
        gettimeofday(&tvNow, NULL); 
        nTimeTest = 
            ((tvNow.tv_sec - tvBegin.tv_sec) * 1000000) +
             (tvNow.tv_usec - tvBegin.tv_usec);
        nReduce = (nTimeTest - nDelay);
        fprintf(
            stderr,
            "   usleep      %8u      %8u  %8d\n",
            nDelay,
            nTimeTest,
            nReduce
        );


        /* [2] test nanosleep */
        gettimeofday(&tvBegin, NULL);
        req.tv_sec = nDelay / 1000000;
        req.tv_nsec = (nDelay % 1000000) * 1000;
        ret = nanosleep(&req, NULL);
        if (-1 == ret)
        { 
            fprintf(
                stderr,
                "   nanosleep   %8u      (not support)\n",
                nDelay
            );
        }
        else
        {
            gettimeofday(&tvNow, NULL); 
            nTimeTest = 
                ((tvNow.tv_sec - tvBegin.tv_sec) * 1000000) +
                 (tvNow.tv_usec - tvBegin.tv_usec);
            nReduce = (nTimeTest - nDelay);
            fprintf(
                stderr,
                "   nanosleep   %8u      %8u  %8d\n",
                nDelay,
                nTimeTest,
                nReduce
            );
        } 


        /* [3] test select */
        gettimeofday(&tvBegin, NULL);
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = nDelay;
        ret = select(0, NULL, NULL, NULL, &tv);
        if (-1 == ret)
        { 
            fprintf(
                stderr,
                "   select        error        .           errno=%d [%s]\n",
                errno,
                strerror(errno)
            );
        } 
        gettimeofday(&tvNow, NULL);
        nTimeTest = 
            ((tvNow.tv_sec - tvBegin.tv_sec) * 1000000) +
             (tvNow.tv_usec - tvBegin.tv_usec);
        nReduce = (nTimeTest - nDelay);
        fprintf(
            stderr,
            "   select      %8u      %8u  %8d\n",
            nDelay,
            nTimeTest,
            nReduce
        );


        /* [4] test pthread */
        pthread_mutex_lock( &mutex );
        gettimeofday(&tvBegin, NULL); 
        req.tv_sec  = tvBegin.tv_sec;
        req.tv_nsec = ((tvBegin.tv_usec + nDelay) * 1000);
        ret = pthread_cond_timedwait(&cond, &mutex, &req);
        if (ETIMEDOUT == ret)
        {
            gettimeofday(&tvNow, NULL); 
            nTimeTest = 
                ((tvNow.tv_sec - tvBegin.tv_sec) * 1000000) +
                 (tvNow.tv_usec - tvBegin.tv_usec);
            nReduce = (nTimeTest - nDelay);
            fprintf(
                stderr,
                "   pthread     %8u      %8u  %8d\n",
                nDelay,
                nTimeTest,
                nReduce
            );
        }
        else if (0 != ret)
        { 
            fprintf(
                stderr,
                "   pthread       error        .           errno=%d\n",
                ret
            );
        }
        pthread_mutex_unlock( &mutex );
    }

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    return 0;
}

