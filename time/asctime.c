#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * 1345600000 ==> Wed Aug 22 09:46:40 2012
 */

int main(int argc, char *argv[])
{
    struct tm *nPtr; 
    time_t t = 0;

    if (argc < 2)
    {
        printf("Usage: asctime INTEGER_VALUE\n");
        printf("\n");
        return 0;
    }

    t = (time_t)atoi( argv[1] );
    nPtr = localtime( &t ); 

    printf("%s", asctime( nPtr ));

    return 0;
}

