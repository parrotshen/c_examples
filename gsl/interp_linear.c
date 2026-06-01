#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_interp.h>

int main(void)
{
    struct timespec ts1, ts2;
    unsigned long td;

    double x[10];
    double y[10];
    double xi;
    double yi;
    int i;
    int j;
    
    
    printf ("x[i] y[i]\n");
    for (i=0, j=1; i<10; i++, j++)
    {
        x[i] = j;
        y[i] = sin(x[i]);
        printf (" %.1f  %+f\n", x[i], y[i]);
    }
    printf ("\n");
    
    printf ("Linear Interpolation\n");
    {
        gsl_interp_accel *acc;
        gsl_interp *linear;
    
        clock_gettime(CLOCK_MONOTONIC, &ts1);

        acc = gsl_interp_accel_alloc ();
        linear = gsl_interp_alloc (gsl_interp_linear, 10);
    
        gsl_interp_init (linear, x, y, 10);
    
        for (i=0; i<10; i++)
        {
            xi = x[i];
            yi = gsl_interp_eval (linear, x, y, xi, acc);
            printf ("%.1f  %+f\n", xi, yi);
            if (i < 9)
            {
                xi = x[i] + 0.3;
                yi = gsl_interp_eval (linear, x, y, xi, acc);
                printf ("%.1f  %+f\n", xi, yi);
                xi = x[i] + 0.6;
                yi = gsl_interp_eval (linear, x, y, xi, acc);
                printf ("%.1f  %+f\n", xi, yi);
            }
        }
        gsl_interp_free (linear);
        gsl_interp_accel_free (acc);

        clock_gettime(CLOCK_MONOTONIC, &ts2);
    }

    td = ((ts2.tv_sec * 1000000000L) + ts2.tv_nsec) -
         ((ts1.tv_sec * 1000000000L) + ts1.tv_nsec);
    printf("\nTime spent: %lu ns\n\n", td);

    return 0;
}

