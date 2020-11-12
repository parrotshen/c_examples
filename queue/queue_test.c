#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

typedef struct _tElement
{
    unsigned char data[256];
    unsigned int  size;
} tElement;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void show_element(void *pObj, int index)
{
    tElement *pElement = pObj;

    printf("[%d] ", index);
    if ( pElement )
    {
        printf("%s", (char *)pElement->data);
    }
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
                queue_dump( show_element );
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

