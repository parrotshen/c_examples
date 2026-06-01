#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_fft_complex.h>
#include <math.h>


int main(void)
{
    gsl_fft_complex_wavetable *wavetable;
    gsl_fft_complex_workspace *workspace;
    double samples[20][2];
    int N = 20;
    int n;

    for (n=0; n<N; n++)
    {
        samples[n][0] = cos(2*M_PI*3*n/N);
        samples[n][1] = 0;
        printf("%d %+f\n", n, samples[n][0]);
    }
    printf("\n");

    wavetable = gsl_fft_complex_wavetable_alloc( N );
    workspace = gsl_fft_complex_workspace_alloc( N );

    gsl_fft_complex_forward((double *)samples, 1, N, wavetable, workspace);

    gsl_fft_complex_wavetable_free( wavetable );
    gsl_fft_complex_workspace_free( workspace );

    for (n=0; n<N; n++)
    {
        printf("%+f %+fi\n", samples[n][0], samples[n][1]);
    }
    printf("\n");

    return 0;
}
