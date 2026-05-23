#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int cmpfunc(const void *a, const void *b)
{
    if (*((double *)a) > *((double *)b)) return 1;
    else if (*((double *)a) < *((double *)b)) return -1;
    else return 0;
}

int main(int argc, char *argv[])
{
    double data[10] = {
        369.11,
        401.82,
        118.04,
        529.52,
        431.00,
        666.66,
        822.74,
        165.45,
        499.20,
        737.39
    };
    int i;

    printf("Before:\n");
    for (i=0; i<10; i++)
    {
        printf(" %.2f", data[i]);
    }
    printf("\n");

    qsort(data, 10, sizeof(double), cmpfunc);

    printf("After:\n");
    for (i=0; i<10; i++)
    {
        printf(" %.2f", data[i]);
    }
    printf("\n");

    return 0;
}
