#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *nptr = "588168";
    int base = 10;

    long valL;
    long long valLL;
    unsigned long valUL;
    unsigned long long valULL;
    float valF;
    double valD;


    printf("\n");
    if (argc > 1)
    {
        nptr = argv[1];
    }

    valL = strtol(nptr, NULL, base);
    printf("strtol(\"%s\", NULL, %d) = %ld\n\n", nptr, base, valL);

    valLL = strtoll(nptr, NULL, base);
    printf("strtoll(\"%s\", NULL, %d) = %lld\n\n", nptr, base, valLL);

    valUL = strtoul(nptr, NULL, base);
    printf("strtoul(\"%s\", NULL, %d) = %lu\n\n", nptr, base, valUL);

    valULL = strtoull(nptr, NULL, base);
    printf("strtoull(\"%s\", NULL, %d) = %llu\n\n", nptr, base, valULL);

    valF = strtof(nptr, NULL);
    printf("strtof(\"%s\", NULL) = %f\n\n", nptr, valF);

    valD = strtod(nptr, NULL);
    printf("strtod(\"%s\", NULL) = %lf\n\n", nptr, valD);

    return 0;
}
