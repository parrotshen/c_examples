#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: remainder X Y\n\n");
        return 0;
    }

    {
        int x = atoi( argv[1] );
        int y = atoi( argv[2] );
        int r;

        r = (x % y);
        printf("\n%d %% %d -> remainder %d\n\n", x, y, r);
    }

    {
        double x = strtod(argv[1], NULL);
        double y = strtod(argv[2], NULL);
        double r;

        r = fmod(x, y);
        printf("fmod(%f, %f) -> remainder %f\n\n", x, y, r);
    }

    {
        double x = strtod(argv[1], NULL);
        double y = strtod(argv[2], NULL);
        double r;

        r = remainder(x, y);
        printf("remainder(%f, %f) -> remainder %f\n\n", x, y, r);
    }

    {
        #define C_REM(a, b) ((a) - (round((double)(a) / (double)(b)) * (b)))
        double x = strtod(argv[1], NULL);
        double y = strtod(argv[2], NULL);
        double r;

        r = C_REM(x, y);
        printf("%f - round(%f / %f) * %f -> remainder %f\n\n", x, x, y, y, r);
    }

    {
        double x = strtod(argv[1], NULL);
        double y = strtod(argv[2], NULL);
        double r;
        int q;

        r = remquo(x, y, &q);
        printf("remquo(%f, %f, &q) -> quotient %d, remainder %f\n\n", x, y, q, r);
    }

    {
        int x = atoi( argv[1] );
        int y = atoi( argv[2] );
        div_t z;

        z = div(x, y);
        printf("div(%d, %d) -> quotient %d, remainder %d\n\n", x, y, z.quot, z.rem);
    }

    {
        long x = strtol( argv[1], NULL, 10 );
        long y = strtol( argv[2], NULL, 10 );
        ldiv_t z;

        z = ldiv(x, y);
        printf("ldiv(%ld, %ld) -> quotient %ld, remainder %ld\n\n", x, y, z.quot, z.rem);
    }

    {
        long long x = strtoll( argv[1], NULL, 10 );
        long long y = strtoll( argv[2], NULL, 10 );
        lldiv_t z;

        z = lldiv(x, y);
        printf("lldiv(%lld, %lld) -> quotient %lld, remainder %lld\n\n", x, y, z.quot, z.rem);
    }

    {
        /* Matlab: b = mod(a, m)
         *           = a - m.*floor(a./m)
         */
        #define MATLAB_MOD(a, m) ((a) - ((m) * floor((double)(a) / (double)(m))))
        double x = strtod(argv[1], NULL);
        double y = strtod(argv[2], NULL);
        double r;

        r = MATLAB_MOD(x, y);
        printf("%f - %f * floor(%f / %f) -> modulo %f\n\n", x, y, x, y, r);
    }

    return 0;
}

