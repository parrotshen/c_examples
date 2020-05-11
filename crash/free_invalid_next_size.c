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

    /*** glibc detected *** ./free_error: free(): invalid next size (fast): 0x0955d008 ***/
    {
        p = malloc( 8 );
        dump(p, 256);

        // write the memory buffer > 8 bytes
        strncpy(p, "abcdefghijklmnopqrstuvwxyz", 256);
        printf("p = %s\n", p);

        // error was happened
        free( p );
    }

    return 0;
}

