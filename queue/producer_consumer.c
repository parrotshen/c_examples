#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>
#include "queue.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

typedef struct _tItem
{
    int  no;
    char name[80];
} tItem;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

pthread_attr_t tattr;
pthread_t tid_p;
pthread_t tid_c;

sem_t avail;

int stop = 0;

char *itemName[10] = {
    "bicycle",
    "book",
    "computer",
    "hat",
    "lamp",
    "refrigerator",
    "shoes",
    "table",
    "television",
    "water"
};


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void signal_hdlr(int sig)
{
    pthread_cancel( tid_p );
    pthread_cancel( tid_c );
}

int rand_number(void)
{
    struct timeval tv;
    int min = 100000; // 100ms
    int max = 10000000; // 10s
    int val;

    gettimeofday(&tv, NULL);

    srand( (unsigned int)tv.tv_usec );
    val = ((rand() % (max - min + 1)) + min);

    return val;
}

void free_element(void *pObj)
{
    tItem *pItem = pObj;

    free( pItem );
}

void show_element(void *pObj, int index)
{
    tItem *pItem = pObj;

    printf("[%d] ", index);
    if ( pItem )
    {
        printf("%d (%s)", pItem->no, pItem->name);
    }
    printf("\n");
}

void *producer(void *arg)
{
    tItem *pItem = NULL;
    int no = 1;
    int val;
    int i;

    for (i=0; i<20; i++)
    {
        val = rand_number();
        /* a piece of time to produce */
        usleep( val );

        pItem = malloc( sizeof( tItem ) );
        if ( pItem )
        {
            printf(
                "[1;31m%s[0m ... %d (%s)\n",
                __func__,
                no,
                itemName[val % 10]
            );
            pItem->no = no++;
            strcpy(pItem->name, itemName[val % 10]);
            if (0 == queue_put( pItem ))
            {
                sem_post( &avail );
            }
            else
            {
                free_element( pItem );
            }
        }
    }

    sem_post( &avail );
    usleep( rand_number() );
    stop = 1;

    pthread_exit(0);
}

void *consumer(void *arg)
{
    tItem *pItem = NULL;

    while ( 1 )
    {
        if (sem_wait( &avail ) != 0)
        {
            printf("%s: sem_wait ... failed\n", __func__);
            break;
        }

        if ( stop ) break;

        pItem = queue_get();
        if ( pItem )
        {
            printf(
                "[1;33m%s[0m ... %d (%s)\n",
                __func__,
                pItem->no,
                pItem->name
            );
            free_element( pItem );

            /* a piece of time to consume */
            usleep( rand_number() );
        }
    }

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int retval;

    signal(SIGINT,  signal_hdlr);
    signal(SIGKILL, signal_hdlr);
    signal(SIGTERM, signal_hdlr);

    sem_init(&avail, 0, 0);

    queue_init(free_element, show_element);

    retval = pthread_attr_init( &tattr );
    retval = pthread_create(&tid_p, &tattr, producer, NULL);
    if (retval != 0)
    {
        printf("ERR: fail to create the producer thread\n");
        return -1;
    }

    retval = pthread_attr_init( &tattr );
    retval = pthread_create(&tid_c, &tattr, consumer, NULL);
    if (retval != 0)
    {
        printf("ERR: fail to create the consumer thread\n");
        pthread_cancel( tid_p );
        return -1;
    }

    pthread_join(tid_p, NULL);
    pthread_join(tid_c, NULL);

    queue_dump();

    queue_cleanup();

    sem_destroy( &avail );

    return 0;
}

