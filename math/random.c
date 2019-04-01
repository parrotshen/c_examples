#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


int random_number(int min, int max)
{
    struct timeval tv;
    int retval = 0;
    int delta;

    delta = (max - min);
    if (delta > 0)
    {
        gettimeofday(&tv, NULL);

        srand( (unsigned int)tv.tv_usec );

        /* get a random number between min and max */
        retval = ((rand() % (delta + 1)) + min);
    }

    return retval;
}

int main(int argc, char *argv[])
{
    int begin  = 0;
    int end    = 1;
    int number = 1;
    int i;
    int r;


    if (argc > 3)
    {
        begin  = atoi( argv[1] );
        end    = atoi( argv[2] );
        number = atoi( argv[3] );
    }
    else if (argc > 2)
    {
        begin  = atoi( argv[1] );
        end    = atoi( argv[2] );
    }
    else
    {
        printf("Usage: random BEGIN END [NUMBER]\n");
        printf("\n");
        return 0;
    }


    printf("RAND_MAX = %d (%Xh)\n\n", RAND_MAX, RAND_MAX);

    for (i=0; i<number; i++)
    {
        r = random_number(begin, end);
        printf("%d\n", r);
    }
    printf("\n");


    return 0;
}

