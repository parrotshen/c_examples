#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_vector.h>


int main(void)
{
    int n1[3] = { 1, 0, 3 };
    int n2[3] = { 2, 7, 5 };
    gsl_vector *v1, *v2;


    v1 = gsl_vector_alloc( 3 );
    v2 = gsl_vector_alloc( 3 );

    gsl_vector_set(v1, 0, n1[0]);
    gsl_vector_set(v1, 1, n1[1]);
    gsl_vector_set(v1, 2, n1[2]);

    gsl_vector_set(v2, 0, n2[0]);
    gsl_vector_set(v2, 1, n2[1]);
    gsl_vector_set(v2, 2, n2[2]);

    gsl_vector_mul(v1, v2);


    printf("  %2d %2d %2d\n", n1[0], n1[1], n1[2]);
    printf("   x  x  x\n");
    printf("  %2d %2d %2d\n", n2[0], n2[1], n2[2]);
    printf("= %2d %2d %2d\n",
           (int)gsl_vector_get(v1, 0),
           (int)gsl_vector_get(v1, 1),
           (int)gsl_vector_get(v1, 2));


    gsl_vector_free( v1 );
    gsl_vector_free( v2 );


    return 0;
}
