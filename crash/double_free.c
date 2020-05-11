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

    /*** glibc detected *** ./free_error: double free or corruption (fasttop): 0x08648008 ***/
    {
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

