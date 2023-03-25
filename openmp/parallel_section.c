#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int N = 8;

    #pragma omp parallel
    {
        int t = omp_get_thread_num();

        #pragma omp sections
        {
            #pragma omp section
            {
                printf("1st section:\n");
                for (int i=0; i<N/2; i++)
                {
                    printf("i = %d (thread %d)\n", i, t);
                }
            }
            #pragma omp section
            {
                printf("2nd section:\n");
                for (int i=N/2; i<N; i++)
                {
                    printf("i = %d (thread %d)\n", i, t);
                }
            }
        }
    }

    return 0;
}

