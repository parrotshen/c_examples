#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PT_SIZE sizeof(void *)

void dump(char *name, void *addr, unsigned int offset, unsigned int size)
{
    unsigned char *p = (addr + offset);
    unsigned int   i;

    if (p == NULL)
    {
        printf("NULL pointer\n\n");
        return;
    }

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % PT_SIZE) == 0))
        {
            if ((p + i - PT_SIZE) == addr)
            {
                printf(" <- %s", name);
            }
            printf("\n");
        }

        if ((i % PT_SIZE) == 0)
        {
            printf("%08lx :", (unsigned long)(p + i));
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
    #if 0
    strncpy(buffer, string, 7);
    buffer[7] = 0;
    #else
    strcpy(buffer, string);
    #endif

    dump("buffer", buffer, -16, 64);

    printf("%s\n", buffer);

    // segmentation fault due to the incorrect return address
}

int main(void)
{
    print( "This string is too long !" );

    return 0;
}

