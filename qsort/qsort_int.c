#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int cmpfunc(const void *a, const void *b)
{
    return (*((int *)a) - *((int *)b));
}

int main(int argc, char *argv[])
{
    int data[10] = {
        99, 89, 79, 55, 69, 59, 49, 19, 29, 39
    };
    int i;

    printf("Before:\n");
    for (i=0; i<10; i++)
    {
        printf(" %2d", data[i]);
    }
    printf("\n");

    qsort(data, 10, sizeof(int), cmpfunc);

    printf("After:\n");
    for (i=0; i<10; i++)
    {
        printf(" %2d", data[i]);
    }
    printf("\n");

    return 0;
}
