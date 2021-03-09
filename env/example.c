#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>

/*
* NOTE:
*   The scope of environment variable is only in this program.
*/

int main(int argc, char *argv[])
{
    char *pName = "my_env";
    char *pValue = "local scope";
    char *pStr;


    if (argc > 2)
    {
        pName = argv[1];
        pValue = argv[2];
    }
    else if (argc > 1)
    {
        pName = argv[1];
    }
    printf("\n");


    /* getenv */
    pStr = getenv( pName );

    printf("[ getenv ]\n");
    if ( pStr ) printf("%s=\"%s\"\n\n", pName, pStr);
    else        printf("%s=%s\n\n", pName, pStr);


    /* setenv */
    if (0 != setenv(pName, pValue, 1))
    {
        perror( "setenv" );
        return -1;
    }

    pStr = getenv( pName );

    printf("[ setenv ]\n");
    if ( pStr ) printf("%s=\"%s\"\n\n", pName, pStr);
    else        printf("%s=%s\n\n", pName, pStr);


    /* unsetenv */
    if (0 != unsetenv( pName ))
    {
        perror( "unsetenv" );
        return -1;
    }

    printf("[ unsetenv ]\n");
    printf("%s=%s\n\n", pName, getenv( pName ));


    return 0;
}

