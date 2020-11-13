#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
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

char *itemName[8] = {
    "bicycle",
    "book",
    "computer",
    "lamp",
    "refrigerator",
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

    for (i=0; i<30; i++)
    {
        val = rand_number();
        usleep( val );

        pItem = malloc( sizeof( tItem ) );
        if ( pItem )
        {
            printf("%s ... %d (%s)\n", __func__, no, itemName[val & 0x7]);
            pItem->no = no++;
            strcpy(pItem->name, itemName[val & 0x7]);
            queue_put( pItem );
        }
    }

    pthread_exit(0);
}

void *consumer(void *arg)
{
    tItem *pItem = NULL;
    int i;

    for (i=0; i<30; i++)
    {
        usleep( rand_number() );

        pItem = queue_get();
        if ( pItem )
        {
            printf("%s ... %d (%s)\n", __func__, pItem->no, pItem->name);
            free_element( pItem );
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

    queue_init();

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

    queue_dump( show_element );

    queue_cleanup( free_element );

    return 0;
}

