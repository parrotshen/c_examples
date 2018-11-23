#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    time_t sec;

    sec = time( NULL );

    printf("%ld (seconds since January 1, 1970)\n", sec);

    return 0;
}

