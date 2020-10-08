#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int uid = getuid();

    /* using root permission to setuid(0) */
    printf("Set uid to 0 ...\n");
    if (setuid(0) != 0)
    {
        perror( "setuid (root)" );
    }
    printf("Who am I?\n");
    system( "whoami" );
    printf("\n");

    printf("Set uid to %d ...\n", uid);
    if (setuid( uid ) != 0)
    {
        perror( "setuid (user)" );
    }
    printf("Who am I?\n");
    system( "whoami" );
    printf("\n");

    return 0;
}
