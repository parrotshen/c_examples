#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#ifndef PRINT_IMMEDIATELY
#define PRINT_IMMEDIATELY 0
#endif


int main(int argc, char *argv[])
{
    printf("Knowledge ");
    #if (PRINT_IMMEDIATELY)
    fflush(stdout);
    #endif

    sleep(1);

    printf("is ");
    #if (PRINT_IMMEDIATELY)
    fflush(stdout);
    #endif

    sleep(1);

    printf("power ");
    #if (PRINT_IMMEDIATELY)
    fflush(stdout);
    #endif

    sleep(1);

    printf("!");
    #if (PRINT_IMMEDIATELY)
    fflush(stdout);
    #endif

    sleep(1);

    /* stdout stream is buffered until reaches a newline */
    printf("\n");


    return 0;
}

