#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_deriv.h>

static double fx(double x, void *pParams)
{
    printf("x = %f\n", x);
    return x;
}

int main(void)
{
    double x[19] = {
        30, 50, 13, 1, 4, 16, 19, 32, 54, 4,
        23, 17, 33, 37, 6, 6, 11, 17, 5
    };
    gsl_function func;
    double result;
    double abserr;
    int i;


    func.function = &fx;
    func.params = 0;

    for (i=0; i<19; i++)
    {
        gsl_deriv_central(&func, x[i], 1, &result, &abserr);
        printf("... %f, %f\n", result, abserr);
    }


    return 0;
}

