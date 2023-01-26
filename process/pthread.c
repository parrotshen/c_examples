#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

pthread_t thread[3];
pthread_attr_t tattr;

void *thread_routine(void *arg)
{
    char ch = *((char *)arg);
    int  loop;
    int  i;

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    loop = ((ch - 'A') + 4);
    printf("thread[%c] ... start (loop %d)\n", ch, loop);

    for (i=0; i<loop; i++)
    {
        pthread_testcancel();
        sleep(1);
        pthread_testcancel();
        printf("thread[%c] ... %d\n", ch, (i + 1));
    }

    printf("thread[%c] ... end\n", ch);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    char id[3] = { 'A', 'B', 'C' };
    int  loop = 5;
    int  error;
    int  i;

    printf("main ........ start (loop %d)\n", loop);

    for (i=0; i<3; i++)
    {
        /* initialized with default attributes */
        pthread_attr_init( &tattr );
        pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);

        /* default behavior specified*/
        error = pthread_create(&(thread[i]), &tattr, thread_routine, &(id[i]));
        if (error != 0)
        {
            printf("ERR: fail to create pthread[%c]\n", id[i]);
            perror( "pthread_create" );
        }

        pthread_attr_destroy( &tattr );
    }

    for (i=0; i<loop; i++)
    {
        sleep(1);
        printf("main ........ %d\n", (i + 1));
    }

    /* do something else for a while */
    if (argc > 1)
    {
        for (i=0; i<3; i++)
        {
            printf("thread[%c] ... cancel\n", id[i]);
            pthread_cancel( thread[i] );
        }
    }
    for (i=0; i<3; i++)
    {
        pthread_join(thread[i], NULL);
    }

    /* it's now safe to use result */
    printf("main ........ end\n");

    return 0;
}

