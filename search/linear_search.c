#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pattern.c"


#define DEBUG 1

#if DEBUG
int g_iteration = 0;
#endif


int linear_search(int A[], int start, int end, int target)
{
    int index = -1;

    if (start > end) return -1;

    #if DEBUG
    printf(" A[%2d] -> %2d\n", start, A[start]);
    g_iteration++;
    #endif

    if (target != A[start])
    {
        index = linear_search(A, (start + 1), end, target);
    }
    else
    {
        #if DEBUG
        printf(" found at A[%d]\n", start);
        #endif
        index = start;
    }

    return index;
}


int main(int argc, char *argv[])
{
    int target = 0;
    int index;

    show_data();

    if (argc > 1)
    {
        target = atoi( argv[1] );
    }
    else
    {
        printf("Usage: linear_search TARGET\n\n");
        return 0;
    }

    index = linear_search(g_data, 0, (g_dataLen - 1), target);

    #if DEBUG
    printf(" iteration(s) ... [1;36m%d[0m\n\n", g_iteration);
    #endif

    if (index >= 0)
    {
        printf(
            "Target [1;33m%d[0m is at A[[1;31m%d[0m]\n\n",
            target,
            index
        );
    }
    else
    {
        printf("ERR: cannot find %d\n\n", target);
    }

    return 0;
}

