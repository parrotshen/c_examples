#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void dump(const void *addr, unsigned int size, int ascii)
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
            printf("%p :", (p + i));
        }
        if (( ascii ) && ((p[i] >= 0x20) && (p[i] <= 0x7E)))
        {
            printf("  [1;36m%c[0m", (char)p[i]);
        }
        else
        {
            printf(" %02x", p[i]);
        }
    }
    printf("\n");
    printf("(%d bytes)\n", size);
    printf("\n");
}

char *string[4] = {
          "Alfa",
          "Bravo",
          "Charlie",
          "Delta"
      };
int num = 4;

int main(void)
{
    int i;

    dump(string, 64, 0);
    for (i=0; i<num; i++)
    {
        dump(string[i], 16, 1);
    }

    printf("\n");
    for (i=0; i<8; i++)
    {
        printf("string[%d] at %p -> %p\n", i, &string[i], string[i]);
    }
    printf("\n");
    for (i=0; i<8; i++)
    {
        printf("string[%d]    %s\n", i, string[i]);
    }
    printf("\n");

    return 0;
}

