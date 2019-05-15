#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEGABYTE (1 << 20)

int main(int argc, char *argv[])
{
    void *pBuf = NULL;
    unsigned int count = 0;

    while (1)
    {
        pBuf = (void *) malloc( MEGABYTE );
        if ( !pBuf ) break;
        /*
        * No more pages are available in the VM
        *   ==> OOM killer
        */
        memset(pBuf, (count & 0xFF), MEGABYTE);
        printf("Currently allocating %d MB\n", ++count);
    }

    printf("Press ENTER to continue ...\n");
    getchar();
    exit(0);
}

