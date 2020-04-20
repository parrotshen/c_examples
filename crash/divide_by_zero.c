#include <stdio.h>
#include <stdlib.h>

int divide(int a, int b)
{
    return a / b;
}

int main(int argc, char *argv[])
{
    int x = 123;
    int y = 0;

    if (argc > 1) y = atoi( argv[1] );

    printf("%d / %d = %d\n", x, y, divide(x, y));

    return 0;
}

