#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define MAX_QUEUE 8


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

typedef struct _tQueue
{
    pthread_mutex_t  lock;
    unsigned short   get;
    unsigned short   put;
    void            *pElement[MAX_QUEUE];
} tQueue;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

static tQueue _queue;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void queue_init(void)
{
    /*
     *        ________________ 
     *       |________________|
     * get ->|________________|
     *       |________________|
     *       |________________|
     *       |________________|<- put
     *       |________________|
     *
     */
    memset(&_queue, 0x00, sizeof( tQueue ));

    pthread_mutex_init(&_queue.lock, NULL);
}

void queue_cleanup(void)
{
    int i;

    for (i=0; i<MAX_QUEUE; i++)
    {
        if ( _queue.pElement[i] )
        {
            free( _queue.pElement[i] );
            _queue.pElement[i] = NULL;
        }
    }

    _queue.get = 0;
    _queue.put = 0;
}

void queue_put(void *pElement)
{
    unsigned short next;

    pthread_mutex_lock( &_queue.lock );

    next = ((_queue.put + 1) % MAX_QUEUE);
    /* Is the queue full ? */
    if (next == _queue.get)
    {
        pthread_mutex_unlock( &_queue.lock );
        printf("%s: queue is full\n", __func__);
        return;
    }

    if ( _queue.pElement[next] )
    {
        printf("%s: pElement[%u] is not NULL\n", __func__, next);
        free( _queue.pElement[next] );
    }
    _queue.pElement[next] = pElement;

    _queue.put = next;

    pthread_mutex_unlock( &_queue.lock );
}

void *queue_get(void)
{
    void *pElement = NULL;
    unsigned short next;

    pthread_mutex_lock( &_queue.lock );

    /* Is the queue empty ? */
    if (_queue.get == _queue.put)
    {
        pthread_mutex_unlock( &_queue.lock );
        printf("%s: queue is empty\n", __func__);
        return NULL;
    }

    next = ((_queue.get + 1) % MAX_QUEUE);

    pElement = _queue.pElement[next];
    _queue.pElement[next] = NULL;

    _queue.get = next;

    pthread_mutex_unlock( &_queue.lock );

    return pElement;
}

int queue_elements(void)
{
    int number = 0;

    pthread_mutex_lock( &_queue.lock );

    /* calculate how many elements in the queue */
    if (_queue.put >= _queue.get)
    {
        number = (_queue.put - _queue.get);
    }
    else
    {
        number = ((_queue.put + MAX_QUEUE) - _queue.get);
    }

    pthread_mutex_unlock( &_queue.lock );

    return number;
}

void queue_dump(tQueueDump pFunc)
{
    int next;
    int number;
    int i;

    number = queue_elements();
    printf("Queue element number = %d\n", number);

    pthread_mutex_lock( &_queue.lock );

    next = ((_queue.get + 1) % MAX_QUEUE);
    for (i=0; i<number; i++)
    {
        pFunc(_queue.pElement[next], next);
        next = ((next + 1) % MAX_QUEUE);
    }

    pthread_mutex_unlock( &_queue.lock );

    printf("\n");
}

