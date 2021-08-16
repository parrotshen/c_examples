#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
*    root(x, n) = r
* => x = r ^ n
* => logr( x ) = logr(r ^ n) = n
* => ln( x ) / ln( r ) = n
* => ln( x ) / n = ln( r )
* => r = e ^ (ln(x) / n)
*/
int main(int argc, char *argv[])
{
    double r;
    double x;
    int n;

    if (argc != 3)
    {
        printf("Usage: nthroot INPUT POWER\n\n");
        return 0;
    }

    x = strtod(argv[1], NULL);
    n = atoi( argv[2] );
    r = exp(log( x ) / n);

    printf("%lf ^ %d = %lf\n\n", r, n, x);

    return 0;
}

