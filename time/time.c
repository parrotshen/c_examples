#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    time_t now;

    now = time( NULL );

    printf("%ld (seconds since January 1, 1970)\n", now);

    return 0;
}

