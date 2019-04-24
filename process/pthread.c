#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_attr_t tattr;
pthread_t tid[3];

void *thread_routine(void *arg)
{
    char ch = *((char *)arg);
    int  loop;
    int  i;

    loop = ((ch - 'A') + 8);
    for (i=0; i<loop; i++)
    {
        printf("thread[%c] ... %d\n", ch, i);
        sleep(1);
    }

    printf("thread[%c] ... end\n", ch);
    pthread_exit(0);
}

int main(void)
{
    char arg[3] = { 'A', 'B', 'C' };
    int  retval;
    int  i;

    printf("main ........ start\n");

    for (i=0; i<3; i++)
    {
        /* initialized with default attributes */
        retval = pthread_attr_init( &tattr );

        /* default behavior specified*/
        retval = pthread_create(&(tid[i]), &tattr, thread_routine, &(arg[i]));
        if (retval != 0)
        {
            printf("ERR: fail to create pthread[%c] (%d)\n", arg[i], retval);
        }
    }

    for (i=0; i<5; i++)
    {
        printf("main ........ %d\n", i);
        sleep(1);
    }

    /* do something else for a while */
    for (i=0; i<3; i++)
    {
        pthread_join(tid[i], NULL);
    }

    /* it's now safe to use result */
    printf("main ........ end\n");

    return 0;
}

