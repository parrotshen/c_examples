#include <stdio.h>
#include <stdlib.h>


typedef struct _tComplex
{
    int  real;
    int  imag;
} tComplex;


void cadd(tComplex *X, tComplex *Y, tComplex *Z)
{
    /*
    *  (a + bi) + (c + di) = (a + c) + (b + d)i
    */
    Z->real = X->real + Y->real;
    Z->imag = X->imag + Y->imag;
}

void csub(tComplex *X, tComplex *Y, tComplex *Z)
{
    /*
    *  (a + bi) - (c + di) = (a - c) + (b - d)i
    */
    Z->real = X->real - Y->real;
    Z->imag = X->imag - Y->imag;
}

void cmul(tComplex *X, tComplex *Y, tComplex *Z)
{
    /*
    *   (a + bi) * (c + di)
    * = (a*c - b*d) + (b*c + a*d)i
    */
    Z->real = (X->real * Y->real) - (X->imag * Y->imag);
    Z->imag = (X->imag * Y->real) + (X->real * Y->imag);
}

void cdiv(tComplex *X, tComplex *Y, tComplex *Z)
{
    /*
    *   (a + bi) / (c + di)
    * = ((a*c + b*d)/(c*c + d*d)) + ((b*c - a*d)/(c*c + d*d))i
    */
    Z->real
     = ((X->real * Y->real) + (X->imag * Y->imag)) / ((Y->real * Y->real) + (Y->imag * Y->imag));
    Z->imag
     = ((X->imag * Y->real) - (X->real * Y->imag)) / ((Y->real * Y->real) + (Y->imag * Y->imag));
}

int main(int argc, char *argv[])
{
    tComplex X = { .real = 444, .imag = 333 };
    tComplex Y = { .real = 222, .imag = 111 };
    tComplex Z;

    printf("X = %d + (%d)i\n", X.real, X.imag);
    printf("Y = %d + (%d)i\n", Y.real, Y.imag);
    printf("\n");

    printf("Addition:\n");
    cadd(&X, &Y, &Z);
    printf("X + Y = %d + (%d)i\n\n", Z.real, Z.imag);

    printf("Subtraction:\n");
    csub(&X, &Y, &Z);
    printf("X - Y = %d + (%d)i\n\n", Z.real, Z.imag);

    printf("Multiplication:\n");
    cmul(&X, &Y, &Z);
    printf("X * Y = %d + (%d)i\n\n", Z.real, Z.imag);

    printf("Divison:\n");
    cdiv(&X, &Y, &Z);
    printf("X / Y = %d + (%d)i\n\n", Z.real, Z.imag);

    return 0;
}

