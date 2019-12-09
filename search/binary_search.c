#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define DEBUG 0


int data[] = {
    1, 3, 4, 6, 7, 8, 10, 13, 14, 19
};


int binary_search(int A[], int T, int L, int R)
{
    int retval = -1;
    int m;

    if (L > R) return retval;

    m = (((R - L) >> 1) + L);
    #if DEBUG
    printf(" A[%d..%d..%d] -> %2d\n", L, m, R, A[m]);
    #endif

    if (A[m] < T)
    {
        L = (m + 1);
        retval = binary_search(A, T, L, R);
    }
    else if (A[m] > T)
    {
        R = (m - 1);
        retval = binary_search(A, T, L, R);
    }
    else
    {
        #if DEBUG
        printf(" found at A[%d]\n", m);
        #endif
        return m;
    }

    return retval;
}

int main(int argc, char *argv[])
{
    int n = (sizeof( data ) / sizeof( int ));
    int m;
    int target = 0;
    int L;
    int R;
    int i;

    for (i=0; i<n; i++)
    {
        printf("A[%d] ", i);
    }
    printf("\n");
    for (i=0; i<n; i++)
    {
        printf(" %2d  ", data[i]);
    }
    printf("\n");
    printf("\n");

    if (argc > 1)
    {
        target = atoi( argv[1] );
    }
    else
    {
        printf("Usage: binary_search TARGET\n\n");
        return 0;
    }

    L = 0;
    R = (n - 1);
    m = binary_search(data, target, L, R);

    #if DEBUG
    printf("\n");
    #endif

    if (m >= 0)
    {
        printf(
            "Target [1;33m%d[0m is at A[[1;31m%d[0m]\n\n",
            target,
            m
        );
    }
    else
    {
        printf("ERR: cannot find %d\n\n", target);
    }

    return 0;
}

