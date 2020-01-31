#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#if 0

typedef struct _tBuffer
{
    struct _tBuffer *next;
} tBuffer;

tBuffer *g_head = NULL;

void *myalloc(int size)
{
    tBuffer *node;
    int size2;

    size2 = (sizeof( tBuffer ) + size);
    node = malloc( size2 );
    if (NULL == node) return NULL;

    memset(node, 0, size2);
    if (NULL == g_head)
    {
        g_head = node;
    }
    else
    {
        node->next = g_head;
        g_head = node;
    }

    return (((void *)node) + sizeof( tBuffer ));
}

void myfree(void)
{
    tBuffer *curr = g_head;
    tBuffer *next;

    while ( curr )
    {
        next = curr->next;
        free( curr );
        curr = next;
    }

    g_head = NULL;
}

char *int2str(int value)
{
    char *string;

    string = myalloc( 32 );

    printf("int2str: %d (%p)\n", value, string);

    sprintf(string, "%d", value);

    return string;
}

int main(int argc, char *argv[])
{
    printf("[CORRECT] return allocated memory\n\n");

    printf(
        "\n%s  %s  %s  %s  %s  %s  %s  %s\n\n",
        int2str( 1 ),
        int2str( 2 ),
        int2str( 3 ),
        int2str( 4 ),
        int2str( 5 ),
        int2str( 6 ),
        int2str( 7 ),
        int2str( 8 )
    );

    myfree();

    return 0;
}

#else

char *int2str(int value)
{
    char string[32];

    printf("int2str: %d (%p)\n", value, string);

    sprintf(string, "%d", value);

    return string;
}

int main(int argc, char *argv[])
{
    printf("[WRONG] return local variable\n\n");

    printf(
        "\n%s  %s  %s  %s  %s  %s  %s  %s\n\n",
        int2str( 1 ),
        int2str( 2 ),
        int2str( 3 ),
        int2str( 4 ),
        int2str( 5 ),
        int2str( 6 ),
        int2str( 7 ),
        int2str( 8 )
    );

    return 0;
}

#endif
