#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#ifndef M_PI
const double M_PI = 3.14159265358979;
#endif

/*
*  Sine wave:
*
*    A Amplitude
*    W Angle frequency (radian/s)
*    F Frequency (Hz)
*    T Period (second)
*    P Phase (radian)
*
*    x(t) = A*sin(W*t + P)
*         = A*sin(2*pi*F*t + P)
*         = A*sin(2*pi/T*t + P)
*/

static void help(void)
{
    printf("Usage: sinewave [OPTION]...\n");
    printf("\n");
    printf("  -a AMPLITUDE\n");
    printf("  -f FREQUENCY (Hz)\n");
    printf("  -p PHASE (degree)\n");
    printf("  -s SAMPLING\n");
    printf("  -t TIME (seconds)\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    int amplitude = 1;
    int frequency = 1;
    int degree = 0;
    double phase = 0;
    int fsample = 64;
    int totaltime = 1;

    FILE *pFile;
    double Ts;
    double t;
    double x;
    int samples;
    int opt;
    int i;


    opterr = 0;
    while ((opt=getopt(argc, argv, "a:f:p:s:t:h")) != -1)
    {
        switch ( opt )
        {
            case 'a':
                amplitude = atoi( optarg );
                break;
            case 'f':
                frequency = atoi( optarg );
                break;
            case 'p':
                degree = atoi( optarg );
                phase = ((degree * M_PI) / 180);
                break;
            case 's':
                fsample = atoi( optarg );
                break;
            case 't':
                totaltime = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    Ts = (1 / (double)fsample);
    samples = ceil(totaltime / Ts);

    printf("Sampling frequency %d (Hz)\n", fsample);
    printf("Frequency          %d (Hz)\n", frequency);
    printf("Phase              %d (degree)\n", degree);
    printf("Amplitude          %d\n", amplitude);
    printf("Total time         %d (sec)\n\n", totaltime);

    if ((pFile=fopen("sinewave.txt", "w")) == NULL)
    {
        printf("ERR: cannot open sinewave.txt\n");
    }

    t = 0;
    for (i=0; i<samples; i++)
    {
        x = amplitude * sin((2 * M_PI * frequency * t) + phase);
        t += Ts;
        if ( pFile ) fprintf(pFile, "%lf %lf\n", t, x);
        if (i < 16) printf("%lf\n", x);
    }

    if ( pFile ) fclose( pFile );

    printf("\n... %d samples\n", samples);
    return 0;
}

