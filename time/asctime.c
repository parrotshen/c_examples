#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 *          0 ==> Thu Jan  1 08:00:00 1970
 *          1 ==> Thu Jan  1 08:00:01 1970
 * 1345600000 ==> Wed Aug 22 09:46:40 2012
 * 0x7fffffff ==> Tue Jan 19 11:14:07 2038
 * 0x80000000 ==> Sat Dec 14 04:45:52 1901
 * 0xffffffff ==> Thu Jan  1 07:59:59 1970
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

    if ((strlen(argv[1]) > 2) &&
        (argv[1][0] == '0') && (argv[1][1] == 'x'))
    {
        sscanf(argv[1], "0x%lx", &t);
    }
    else
    {
        t = (time_t)atoi( argv[1] );
    }

    nPtr = localtime( &t ); 

    printf("%s", asctime( nPtr ));

    return 0;
}

