#include <stdio.h>
#include <stdlib.h>

long gcd(long a, long b)
{
    long r;

    while (b != 0)
    {
        r = a % b;
        a = b;
        b = r;
    }

    return a;
}

long lcm(long a, long b)
{
    return ((a * b) / gcd(a, b));
}

int main(int argc, char *argv[])
{
    long a, b;

    if (argc != 3)
    {
        printf("Usage: gcd_lcm NUMBER_A NUMBER_B\n\n");
        return 0;
    }

    a = (long)atol( argv[1] );
    b = (long)atol( argv[2] );

    printf("\n");
    printf("GCD: %ld\n", gcd(a, b));
    printf("LCM: %ld\n", lcm(a, b));
    printf("\n");

    return 0;
}

