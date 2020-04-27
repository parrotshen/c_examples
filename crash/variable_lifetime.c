#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#if 0

char *int2str(int value)
{
    static char string[32];

    printf("string[] at [1;33m%p[0m (data)\n", string);

    sprintf(string, "%d", value);

    return string;
}

int main(int argc, char *argv[])
{
    printf("[CORRECT] return static variable\n\n");

    printf("\n\"%s\"\n\n", int2str( 1234 ));

    return 0;
}

#else

char *int2str(int value)
{
    char string[32];

    printf("string[] at [1;33m%p[0m (stack)\n", string);

    sprintf(string, "%d", value);

    return string;
}

int main(int argc, char *argv[])
{
    printf("[WRONG] return local variable\n\n");

    printf("\n\"%s\"\n\n", int2str( 1234 ));

    return 0;
}

#endif
