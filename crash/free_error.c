#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dump(const void *addr, unsigned int size)
{
    unsigned char *p = (unsigned char *)addr;
    unsigned int   i;

    if (p == NULL)
    {
        printf("NULL\n\n");
        return;
    }

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }

        if ((i % 16) == 0)
        {
            printf("%08x :", (int)(p + i));
        }
        printf(" %02x", p[i]);
    }
    printf("\n");
    printf("(%d bytes)\n", size);
    printf("\n");
}

int main(int argc, char *argv[])
{
    char *p = NULL;
    int test = 0;

    if (argc > 1) test = atoi( argv[1] );

    /*** glibc detected *** ./free_error: free(): invalid next size (fast): 0x0955d008 ***/
    if (0 == test)
    {
        p = malloc( 8 );
        dump(p, 256);

        // write the memory buffer > 8 bytes
        strncpy(p, "abcdefghijklmnopqrstuvwxyz", 256);
        printf("p = %s\n", p);

        // error was happened
        free( p );
    }
    /*** glibc detected *** ./free_error: double free or corruption (fasttop): 0x08648008 ***/
    else
    {
        char *p = NULL;

        p = malloc( 16 );
        memset(p, 0x5a, 16);
        printf("malloc %p\n", p);
        dump((p - 8), 32);

        // free the allocated memory
        printf("free %p\n", p);
        free( p );
        dump((p - 8), 32);

        // free again and error was happened
        printf("free %p\n", p);
        free( p );
        dump((p - 8), 32);
    }

    return 0;
}

