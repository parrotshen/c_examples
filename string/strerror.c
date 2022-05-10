#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
#if 1
    int errnum;

    for (errnum=0; errnum<256; errnum++)
    {
        printf("[%3d] %s\n", errnum, strerror( errnum ));
    }
#else
    int errnum;
    int i;

    if (argc < 2)
    {
        printf("Usage: strerror ERRNUM...\n\n");
        return 0;
    }

    for (i=1; i<argc; i++)
    {
        errnum = atoi( argv[i] );
        printf("[%d] %s\n", errnum, strerror( errnum ));
    }
#endif

    return 0;
}
