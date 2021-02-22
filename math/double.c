#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


/*
*    1      11          52
*  sign  exponent    fraction
*  +---+-----------+-----------+
*  | x | x x ... x | x x ... x |
*  +---+-----------+-----------+
*   63  62      52  51       0
*/

void dump(unsigned char *byte)
{
    int size = sizeof( double );
    int i, j;

    for (i=0, j=(size-1); i<size; i++, j--)
    {
        printf(" %02x", byte[j]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    #ifdef __TOLERANCE__

    double precision = 0.00001;
    double A = 26523.70;
    double B;
    int i;

    if (argc > 1)
    {
        precision = (double)atof( argv[1] );
    }

    B = 26500.0;
    for (i=0; i<395; i++)
    {
        B += (60.0 / 1000);
    }

    printf("A = %lf\n", A);
    printf("B = %lf\n", B);
    printf("\n");

    if (fabs(A - B) < precision)
    {
        printf("A == B\n");
    }
    else
    {
        printf("A != B\n");
    }
    printf(
        "precision %.*e / %.*e\n",
        10,
        precision,
        10,
        fabs(B - A)
    );
    printf("\n");

    #else

    double A = 26523.70;
    double B;
    int i;

    B = 26500.0;
    for (i=0; i<395; i++)
    {
        B += (60.0 / 1000);
    }

    printf("A = %lf\n", A);
    printf("B = %lf\n", B);
    printf("\n");

    if (A == B)
    {
        printf("A == B\n");
    }
    else
    {
        printf("A != B\n");
        dump( (void *)&A );
        dump( (void *)&B );
    }
    printf("\n");

    #endif


    return 0;
}

