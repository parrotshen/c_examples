#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int cmpfunc(const void *a, const void *b)
{
    while ( *((char *)a) )
    {
        if (*((char *)a) != *((char *)b))
        {
            if (*((char *)a) > 'Z')
            {
                if (*((char *)b) > 'Z') return (*((char *)a) - *((char *)b));
                else return -1;
            }
            else
            {
                if (*((char *)b) > 'Z') return 1;
                else return (*((char *)a) - *((char *)b));
            }
        }
        a++;
        b++;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char data[10][8] = {
        { "Black" },
        { "Red" },
        { "Green" },
        { "Yellow" },
        { "Blue" },
        { "Purple" },
        { "Cyan" },
        { "White" },
        { "Pink" },
        { "Orange" }
    };
    int i;

    printf("Before:\n");
    for (i=0; i<10; i++)
    {
        printf(" %s", data[i]);
    }
    printf("\n");

    qsort(data, 10, 8, cmpfunc);

    printf("After:\n");
    for (i=0; i<10; i++)
    {
        printf(" %s", data[i]);
    }
    printf("\n");

    return 0;
}
