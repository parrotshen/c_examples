#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_QUEUE  8
#define DATA_SIZE  1024

typedef struct _tElement
{
    unsigned char  data[DATA_SIZE];
    unsigned int   size;
} tElement;

typedef struct _tQueue
{
    pthread_mutex_t  lock;
    unsigned short   get;
    unsigned short   put;
    tElement        *pElement[MAX_QUEUE];
} tQueue;

static tQueue  _queue;


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
    unsigned int  i;

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

void queue_put(tElement *pElement)
{
    unsigned short  next;

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
        printf("%s: pElement[%d] is not NULL\n", __func__, next);
        free( _queue.pElement[next] );
    }
    _queue.pElement[next] = pElement;

    _queue.put = next;

    pthread_mutex_unlock( &_queue.lock );
}

tElement *queue_get(void)
{
    tElement *pElement = NULL;
    unsigned short  next;

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

unsigned int queue_elements(void)
{
    unsigned int  number = 0;

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
    unsigned int  next;
    unsigned int  number;
    unsigned int  i;

    number = queue_elements();
    printf("Queue element number = %d\n", number);

    pthread_mutex_lock( &_queue.lock );

    next = ((_queue.get + 1) % MAX_QUEUE);
    for (i=0; i<number; i++)
    {
       printf("[%u] ", next);
       if ( _queue.pElement[next] )
       {
           printf("%s", (char *)_queue.pElement[next]->data);
       }
       printf("\n");
       next = ((next + 1) % MAX_QUEUE);
    }

    pthread_mutex_unlock( &_queue.lock );

    printf("\n");
}

int main(int argc, char *argv[])
{
    tElement *pElement = NULL;
    char  buf[256];
    int   size;

    queue_init();

    while (1)
    {
        size = read(STDIN_FILENO, buf, 255);
        buf[size] = '\0';

        if (strlen(buf) > 0)
        {
            buf[strlen(buf) - 1] = '\0';

            if (0 == strcmp("exit", buf))
            {
                break;
            }
            else if (0 == strcmp("help", buf))
            {
                printf("exit: terminate program\n");
                printf("show: dump queue\n");
                printf("get : de-queue\n");
                printf("put : en-queue (null)\n");
                printf("\n");
            }
            else if (0 == strcmp("show", buf))
            {
                queue_dump();
            }
            else if (0 == strcmp("get", buf))
            {
                pElement = queue_get();
                if ( pElement )
                {
                    printf("%s\n\n", (char *)pElement->data);
                    free( pElement );
                }
            }
            else if (0 == strcmp("put", buf))
            {
                queue_put( NULL );
            }
            else
            {
                pElement = malloc( sizeof( tElement ) );
                if ( pElement )
                {
                    strcpy((char *)pElement->data, buf);
                    pElement->size = strlen( (char *)pElement->data );
                    queue_put( pElement );
                }
            }
        }
    }

    queue_cleanup();

    return 0;
}

