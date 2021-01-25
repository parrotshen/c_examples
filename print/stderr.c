#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    /* stderr stream is un-buffered */
    fprintf(stderr, "Knowledge ");

    sleep(1);

    fprintf(stderr, "is ");

    sleep(1);

    fprintf(stderr, "power ");

    sleep(1);

    fprintf(stderr, "!");

    sleep(1);

    fprintf(stderr, "\n");

    return 0;
}

