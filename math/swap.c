#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char *argv[])
{
    int a = 1234;
    int b = 5678;

    if (3 == argc)
    {
        a = atoi( argv[1] );
        b = atoi( argv[2] );
    }
    else if (2 == argc)
    {
        a = atoi( argv[1] );
    }

    printf("a = %d\n", a);
    printf("b = %d\n\n", b);

    a = a ^ b;
    b = b ^ a;
    a = a ^ b;
    printf("swap:\n");
    printf("a = %d\n", a);
    printf("b = %d\n\n", b);

    return 0;
}

