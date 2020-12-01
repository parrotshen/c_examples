#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pattern.c"


#define DEBUG 1

#if DEBUG
int g_iteration = 0;
#endif


/*
* f(0) f(1) f(2) f(3) f(4) f(5) f(6) f(7) f(8)
*   0    1    1    2    3    5    8   13   21
*                                         /
*                          ____________ 13 ____________
*                      -5 /                            \ +5
*                 ______ 8 ______                 _____ 18 _____
*             -3 /               \ +3         -2 /              \ +2
*           ___ 5 ___             11            16              20
*       -2 /         \ +2     -1 /  \ +1    -1 /  \ +1      -1 /
*         3           7        10   12       15   17         19
*     -1 / \ +1   -1 /      -1 /          -1 /
*       2   4       6         9            14
*   -1 /
*     1
*/

int recursion(int A[], int len, int index, int fib[], int target)
{
    /* do not exceed the array boundary */
    if (index >= len)
    {
        #if DEBUG
        printf(" A[%2d] -> x\n", index);
        g_iteration++;
        #endif

        if (0 == fib[1]) return -1;
        /* go left */
        index -= fib[1];

        /* backward by 1 */
        fib[2] = fib[1];
        fib[1] = fib[0];
        fib[0] = fib[2] - fib[1];

        return recursion(A, len, index, fib, target);
    }

    #if DEBUG
    printf(" A[%2d] -> %2d\n", index, A[index]);
    g_iteration++;
    #endif

    if (target < A[index])
    {
        if (0 == fib[1]) return -1;
        /* go left */
        index -= fib[1];

        /* backward by 1 */
        fib[2] = fib[1];
        fib[1] = fib[0];
        fib[0] = fib[2] - fib[1];

        index = recursion(A, len, index, fib, target);
    }
    else if (target > A[index])
    {
        if (0 == fib[1]) return -1;
        /* go right */
        index += fib[1];

        /* backward by 2 */
        if (0 == fib[0]) return -1;
        fib[2] = fib[0];
        fib[1] = fib[1] - fib[0];
        fib[0] = fib[2] - fib[1];

        index = recursion(A, len, index, fib, target);
    }
    else
    {
        #if DEBUG
        printf(" found at A[%d]\n", index);
        #endif
        ;
    }

    return index;
}

int fibonacci_search(int A[], int len, int target)
{
    int fib[3] = { 0, 1, 1 };
    int index;

    /* find the fibonacci root node */
    while (fib[2] <= len)
    {
        fib[0] = fib[1];
        fib[1] = fib[2];
        fib[2] = fib[0] + fib[1];
    }

    /* index to start comparsion */
    index = fib[1] - 1;

    /* backward by 2 */
    fib[2] = fib[0];
    fib[1] = fib[1] - fib[0];
    fib[0] = fib[2] - fib[1];

    return recursion(A, len, index, fib, target);
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
        printf("Usage: fibonacci_search TARGET\n\n");
        return 0;
    }

    index = fibonacci_search(g_data, g_dataLen, target);

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

