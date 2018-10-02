#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dump(const void *addr, unsigned int size)
{
    unsigned char *p = (unsigned char *)addr;
    unsigned int   i;

    if (p == NULL)
    {
        printf("NULL pointer\n\n");
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

void print(char *string)
{
    char buffer[8];

    // access over the size of buffer
    sprintf(buffer, "%s: %s", __func__, string);

    dump((buffer - 16), 64);

    // cause segmentation fault
    printf("%s\n", buffer);
}

int main(void)
{
    print( "0123456789" );

    return 0;
}

