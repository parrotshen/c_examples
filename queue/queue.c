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
    tQueueCleanup    pCleanup;
    tQueueDump       pDump;
    void            *pElement[MAX_QUEUE];
} tQueue;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

static tQueue _queue;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void queue_init(tQueueCleanup pCleanup, tQueueDump pDump)
{
    /*
     *        ________________ 
     *       |________________|
     *       |________________|<- get
     *       |________________| \
     *       |________________| | queue elements
     * put ->|________________| /
     *       |________________|
     *
     */
    memset(&_queue, 0x00, sizeof( tQueue ));
    _queue.pCleanup = pCleanup;
    _queue.pDump = pDump;

    pthread_mutex_init(&_queue.lock, NULL);
}

void queue_cleanup(void)
{
    int i;

    pthread_mutex_lock( &_queue.lock );

    for (i=0; i<MAX_QUEUE; i++)
    {
        if ( _queue.pElement[i] )
        {
            if ( _queue.pCleanup )
            {
                _queue.pCleanup( _queue.pElement[i] );
            }
            _queue.pElement[i] = NULL;
        }
    }

    _queue.get = 0;
    _queue.put = 0;

    pthread_mutex_unlock( &_queue.lock );
}

int queue_put(void *pObj)
{
    unsigned short next;

    pthread_mutex_lock( &_queue.lock );

    next = ((_queue.put + 1) % MAX_QUEUE);
    /* Is the queue full ? */
    if (next == _queue.get)
    {
        pthread_mutex_unlock( &_queue.lock );
        printf("%s: queue is full\n", __func__);
        return -1;
    }

    if ( _queue.pElement[next] )
    {
        printf("%s: pElement[%u] is not NULL\n", __func__, next);
        if ( _queue.pCleanup )
        {
            _queue.pCleanup( _queue.pElement[next] );
        }
    }

    _queue.pElement[next] = pObj;
    _queue.put = next;

    pthread_mutex_unlock( &_queue.lock );

    return 0;
}

void *queue_get(void)
{
    void *pObj = NULL;
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

    pObj = _queue.pElement[next];
    _queue.pElement[next] = NULL;
    _queue.get = next;

    pthread_mutex_unlock( &_queue.lock );

    return pObj;
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

void queue_dump(void)
{
    int next;
    int number;
    int i;

    if ( _queue.pDump )
    {
        printf("\n");
        number = queue_elements();
        printf("Queue has %d element(s)\n", number);

        pthread_mutex_lock( &_queue.lock );

        next = ((_queue.get + 1) % MAX_QUEUE);
        for (i=0; i<number; i++)
        {
            _queue.pDump(_queue.pElement[next], next);
            next = ((next + 1) % MAX_QUEUE);
        }

        pthread_mutex_unlock( &_queue.lock );

        printf("\n");
    }
}

