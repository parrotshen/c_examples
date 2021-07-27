#include <stdio.h>
#include <stdlib.h>

void factorize(long n)
{
    unsigned long m = abs(n);
    unsigned long i;

    printf("%lu = ", m);
    for (i=2; (i * i)<=abs(m);)
    {
        if (0 == (m % i))
        {
            printf("%lu * ", i);
            m /= i;
        }
        else
        {
            i++;
        }
    }
    printf("%lu\n", m);
}

int main(int argc, char *argv[])
{
    long n = 256;

    if (argc > 1)
    {
        n = (long)atol( argv[1] );
    }

    factorize( n );

    return 0;
}

