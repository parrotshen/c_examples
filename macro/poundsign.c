#include <stdio.h>
#include <stdlib.h>


/* one pound sign: convert to string */
#define TO_STR(arg)  (#arg)

/* two pound sign: concatenate two macros */
#define TO_CAT(arg)  (g_##arg)


int g_1111   = 4444;
int g_number = 8888;

int main(int argc, char *argv[])
{
    int number = 123;

    if (argc > 1)
    {
        number = atoi( argv[1] );
    }

    printf("%s\n", TO_STR(1111));
    printf("%s\n", TO_STR(number));
    printf("\n");

    printf("%d\n", TO_CAT(1111));
    printf("%d\n", TO_CAT(number));
    printf("\n");

    return 0;
}

