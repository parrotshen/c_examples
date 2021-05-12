#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

/*
* Gregory-Leibniz series:
*   pi = 4/1 - 4/3 + 4/5 - 4/7 + 4/9 - 4/11 + 4/13 - ...
*   (22/7 could commonly approximate)
*/
#ifndef M_PI
const double M_PI = 3.14159265358979;
#endif

int main(int argc, char *argv[])
{
    double pi = 0.0;
    double numerator = 4.0;
    double denominator = 1.0;
    int terms = 100;
    int n;

    if (argc > 1)
    {
        terms = atoi( argv[1] );
    }

    for (n=1; n<(terms+1); n++)
    {
        if (n & 0x1)
        {
            pi += (numerator / denominator);
        }
        else
        {
            pi -= (numerator / denominator);
        }
        denominator += 2;
    }

    printf("M_PI  =  %.20f\n", M_PI);
    printf("PI    =  %.20f\n", pi);
    printf("delta = %.20f\n", (pi - M_PI));
    printf("terms = %d\n", terms);

    return 0;
}

