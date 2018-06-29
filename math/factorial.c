#include <stdio.h>
#include <stdlib.h>

unsigned int factorial(unsigned int n)
{
    if (0 == n)
    {
        return 1;
    }
    else
    {
        return (n * factorial(n - 1));
    }
}


int main(int argc, char *argv[])
{
    unsigned int n = 10;
    unsigned int f;

    if (argc > 1)
    {
        n = (unsigned int)atoi( argv[1] );
    }

    f = factorial( n );

    printf("%u! = %u\n", n, f);

    return 0;
}

