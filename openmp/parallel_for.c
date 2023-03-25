#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int threads = 5;

    if (argc > 1) threads = atoi(argv[1]);

    #pragma omp parallel num_threads( threads )
    {
        int t = omp_get_thread_num();

        #pragma omp for
        for (int i=0; i<10; i++)
        {
            printf("i = %d (thread %d)\n", i, t);
            sleep(1);
        }
    }

    return 0;
}

