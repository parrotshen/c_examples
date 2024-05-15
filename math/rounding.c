#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int a = 10;
    int b = 4;
    int c;

    if (argc > 2)
    {
        b = atoi( argv[2] );
        a = atoi( argv[1] );
    }
    else if (argc > 1)
    {
        a = atoi( argv[1] );
    }

    c = (a + (b / 2)) / b;
    printf("%d / %d = %d\n", a, b, c);

    return 0;
}

