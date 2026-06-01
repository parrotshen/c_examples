#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

int main(void)
{
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
        printf ("%.1f  %+f\n", x[i], y[i]);
    }
    printf("\n");
    
    printf ("Spline Interpolation\n");
    {
        gsl_interp_accel *acc;
        gsl_spline *spline;
    
        acc = gsl_interp_accel_alloc ();
        spline = gsl_spline_alloc (gsl_interp_cspline, 10);
    
        gsl_spline_init (spline, x, y, 10);
    
        for (i=0; i<10; i++)
        {
            xi = x[i];
            yi = gsl_spline_eval (spline, xi, acc);
            printf ("%.1f  %+f\n", xi, yi);
            if (i < 9)
            {
                xi = x[i] + 0.3;
                yi = gsl_spline_eval (spline, xi, acc);
                printf ("%.1f  %+f\n", xi, yi);
                xi = x[i] + 0.6;
                yi = gsl_spline_eval (spline, xi, acc);
                printf ("%.1f  %+f\n", xi, yi);
            }
        }
        gsl_spline_free (spline);
        gsl_interp_accel_free (acc);
    }
    printf("\n");

    return 0;
}

