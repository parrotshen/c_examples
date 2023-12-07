#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    {
        double x = 123.123123;
        double y = 88.168;
        double z;

        z = fmod(x, y);
        printf("fmod(%f, %f) -> %f\n\n", x, y, z);
    }

    {
        int x = -1111;
        int y = 22;
        div_t z;

        z = div(x, y);
        printf("div(%d, %d) -> %d, %d\n\n", x, y, z.quot, z.rem);
    }

    {
        long x = 1111;
        long y = -22;
        ldiv_t z;

        z = ldiv(x, y);
        printf("ldiv(%ld, %ld) -> %ld, %ld\n\n", x, y, z.quot, z.rem);
    }

    {
        long long x = 9999999999999;
        long long y = 1234567;
        lldiv_t z;

        z = lldiv(x, y);
        printf("lldiv(%lld, %lld) -> %lld, %lld\n\n", x, y, z.quot, z.rem);
    }

    return 0;
}

