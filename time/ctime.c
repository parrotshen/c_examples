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
    time_t t;

    if (argc < 2)
    {
        printf("Usage: ctime INTEGER_VALUE\n");
        printf("\n");
        t = 0x80000000;
        printf("[0x80000000] %s", ctime( &t ));
        t = 0xffffffff;
        printf("[0xffffffff] %s", ctime( &t ));
        t = 0;
        printf("[         0] %s", ctime( &t ));
        t = 1;
        printf("[         1] %s", ctime( &t ));
        t = 1345600000;
        printf("[1345600000] %s", ctime( &t ));
        t = 0x7fffffff;
        printf("[0x7fffffff] %s", ctime( &t ));
        printf("\n");
        printf("sizeof( time_t ) = %d\n", sizeof( time_t ));
        printf("\n");
        return 0;
    }

    t = strtoul(argv[1], NULL, 0);
    printf("%s", ctime( &t ));

    return 0;
}

