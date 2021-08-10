#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
*  Euler's formula:
*    e ^ (i * x) = cos( x ) + i * sin( x )
*/
int main(int argc, char *argv[])
{
    double x;

    if (argc != 2)
    {
        printf("Usage: euler INPUT\n\n");
        return 0;
    }

    x = strtod(argv[1], NULL);
    printf(
        "e ^ (i * %lf) = %lf + i * %lf\n\n",
        x,
        cos( x ),
        sin( x )
    );

    return 0;
}

