#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

#ifndef M_PI
const double M_PI = 3.14159265358979;
#endif

int main(int argc, char *argv[])
{
    double complex X = 444 + 333*I;
    double complex Y = 222 + 111*I;
    double complex Z;
    double phase;
    double degree;

    printf("X = %f + (%f)i\n", creal(X), cimag(X));
    printf("Y = %f + (%f)i\n", creal(Y), cimag(Y));
    printf("\n");

    printf("Addition:\n");
    Z = X + Y;
    printf("X + Y = %f + (%f)i\n\n", creal(Z), cimag(Z));

    printf("Subtraction:\n");
    Z = X - Y;
    printf("X - Y = %f + (%f)i\n\n", creal(Z), cimag(Z));

    printf("Multiplication:\n");
    Z = X * Y;
    printf("X * Y = %f + (%f)i\n\n", creal(Z), cimag(Z));

    printf("Divison:\n");
    Z = X / Y;
    printf("X / Y = %f + (%f)i\n\n", creal(Z), cimag(Z));

    printf("Conjugate:\n");
    printf("~X = %f + (%f)i\n", creal( conj(X) ), cimag( conj(X) ));
    printf("~Y = %f + (%f)i\n\n", creal( conj(Y) ), cimag( conj(Y) ));

    printf("Absolute:\n");
    printf("|X| = %f\n", cabs(X));
    printf("|Y| = %f\n\n", cabs(Y));

    printf("Phase:\n");
    phase = carg(X);
    degree = (360 * phase) / (2 * M_PI);
    printf("psi(X) = %f (%f degree)\n", phase, degree);
    phase = carg(Y);
    degree = (360 * phase) / (2 * M_PI);
    printf("psi(Y) = %f (%f degree)\n\n", phase, degree);

    return 0;
}

