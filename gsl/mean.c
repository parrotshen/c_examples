#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_statistics_double.h>


int main(void)
{
    double data1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    double data2[10] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3  };
    double data3[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 };

    printf("%.4f\n", gsl_stats_mean(data1, 1, 10));
    printf("%.4f\n", gsl_stats_mean(data2, 1, 10));
    printf("%.4f\n", gsl_stats_mean(data3, 1, 10));

    return 0;
}
