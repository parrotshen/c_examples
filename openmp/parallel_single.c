#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    #pragma omp parallel num_threads(4)
    {
        int t = omp_get_thread_num();

        printf("outside (thread %d)\n", t);

        #pragma omp single
        {
            printf("inside  (thread %d)\n", t);
        }
    }

    return 0;
}

