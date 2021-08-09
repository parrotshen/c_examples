#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char *argv[])
{
    double x;
    double y;
    double z;

    if (argc != 3)
    {
        printf("Usage: powxy BASE EXPONENT\n\n");
        return 0;
    }

    x = strtod(argv[1], NULL);
    y = strtod(argv[2], NULL);
    z = pow(x, y);
    printf("%lf ^ %lf = %lf\n\n", x, y, z);

    return 0;
}

