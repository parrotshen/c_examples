#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>  /* backtrace() */


typedef struct _tData
{
    char dummy[16];
    int  foo;
    int  bar;
} tData;


static void handler(int sig)
{
/* "SIZE" means line size trace information you want to show. */
#define SIZE 100
    void  *buffer[SIZE];
    char **strings;
    int    nptrs;
    int    i;

    if (SIGSEGV == sig)
    {
        printf("segmentation fault\n\n");

        nptrs = backtrace(buffer, SIZE);
        printf("backtrace() returned %d addresses\n", nptrs);

        /*
        * The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
        *   would produce similar output to the following:
        */
        strings = backtrace_symbols(buffer, nptrs);
        if (NULL == strings)
        {
            perror( "backtrace_symbols" );
            exit( EXIT_FAILURE );
        }

        for (i=0; i<nptrs; i++)
        {
            printf("%s\n", strings[i]);
        }
        printf("\n");

        free( strings );

        exit( -1 );
    }
}

void foo_bar(tData *pData, int foo, int bar)
{
    pData->foo = foo;
    pData->bar = bar;
}

int main(int argc, char *argv[])
{
    int foo = 123;
    int bar = 456;

    #if 1
    signal(SIGSEGV, handler);
    #endif

    if (argc > 1)
    {
        foo = atoi( argv[1] );
    }
    if (argc > 2)
    {
        bar = atoi( argv[2] );
    }

    foo_bar(NULL, foo, bar);

    return 0;
}

