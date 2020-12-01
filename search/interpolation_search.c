#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pattern.c"


#define DEBUG 1

#if DEBUG
int g_iteration = 0;
#endif


int interpolation_search(int A[], int start, int end, int target)
{
    /*
    *  (y - Y1) = ((Y2 - Y1) / (X2 - X1)) * (x - X1)
    */
    int index = -1;
    int x, y;

    if (start > end) return -1;

    if (start == end)
    {
        x = start;
    }
    else
    {
        y = target;
        x = ((((y - A[start]) * (end - start)) / (A[end] - A[start])) + start);
        if ((x < start) || (x > end))
        {
            #if DEBUG
            printf(" A[%2d..%2d..%2d] -> x\n", start, x, end);
            g_iteration++;
            #endif
            return -1;
        }
    }

    #if DEBUG
    printf(" A[%2d..%2d..%2d] -> %2d\n", start, x, end, A[x]);
    g_iteration++;
    #endif

    if (target > A[x])
    {
        start = (x + 1);
        index = interpolation_search(A, start, end, target);
    }
    else if (target < A[x])
    {
        end = (x - 1);
        index = interpolation_search(A, start, end, target);
    }
    else
    {
        #if DEBUG
        printf(" found at A[%d]\n", x);
        #endif
        index = x;
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
        printf("Usage: interpolation_search TARGET\n\n");
        return 0;
    }

    index = interpolation_search(g_data, 0, (g_dataLen - 1), target);

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

