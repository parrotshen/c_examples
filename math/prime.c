#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char *argv[])
{
    unsigned long val, i;

    if (argc < 2)
    {
        printf("Usage: prime NUMBER\n\n");
        return 0;
    }

    val = strtoul(argv[1], NULL, 10);
    if (val <= 1) goto _FALSE;

    for (i=2; i<=sqrt(val); i++)
    {
        if (0 == (val % i)) goto _FALSE;
    }

    printf("[1;33m%lu[0m is a prime number\n", val);
    return 1;

_FALSE:
    printf("%lu is not a prime number\n", val);
    return 0;
}

