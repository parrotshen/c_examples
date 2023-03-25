#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* MALLOC_TRACE=report.txt ./leak
*/

int main(int argc, char *argv[])
{
    void *p;

    mtrace();

    p = malloc( 100 );
    memset(p, 0x11, 100);
    printf("malloc ... %p (100 bytes)\n", p);

    free( p );
    printf("free ..... %p\n", p);
    p = NULL;

    p = malloc( 200 );
    memset(p, 0x22, 200);
    printf("malloc ... %p (200 bytes)\n", p);

    muntrace();

    return 0;
}

