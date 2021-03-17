#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h> /* backtrace() */
#include <error.h>


void func_3(void)
{
    void  *pBuf[128];
    char **ppStr;
    int num;
    int i;

    num = backtrace(pBuf, 128);
    printf("backtrace() returned %d addresses\n", num);

    ppStr = backtrace_symbols(pBuf, num);
    if (NULL == ppStr)
    {
        perror( "backtrace_symbols" );
        return;
    }

    for (i=0; i<num; i++)
    {
        printf("%s\n", ppStr[i]);
    }
    printf("\n");

    free( ppStr );
}

/* "static" means don't export the symbol */
static void func_2(void)
{
    func_3();
}

void func_1(int test)
{
    if (test > 1)
    {
        func_1(test - 1);
    }
    else
    {
        func_2();
    }
}

int main(int argc, char *argv[])
{
    int test = 3;

    if (argc > 1)
    {
        test = atoi( argv[1] );
    }

    printf("main   at %p\n", main);
    printf("func_1 at %p\n", func_1);
    printf("func_2 at %p\n", func_2);
    printf("func_3 at %p\n", func_3);
    printf("\n");

    func_1( test );

    return 0;
}

