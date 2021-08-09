#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
*  x ^ y = z
*  logx( z ) = logx(x ^ y) = y
*/
int main(int argc, char *argv[])
{
    double y;
    double z;
    double x;

    if (argc != 3)
    {
        printf("Usage: logx BASE INPUT\n\n");
        return 0;
    }

    x = strtod(argv[1], NULL);
    z = strtod(argv[2], NULL);

    /*
    *  Change of base:
    *    logx( z ) = logk( z ) / logk( x )
    *              = ln( z ) / ln( x )
    */
    y = log( z ) / log( x );

    printf("log%lf( %lf ) = %lf\n\n", x, z, y);

    return 0;
}

