#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h> /* getpwuid */
#include <errno.h>

int main(int argc, char *argv[])
{
    struct passwd *pw;
    uid_t uid;
    uid_t euid;


    uid = getuid();
    euid = geteuid();
    pw = getpwuid( uid );
    if ( pw )
    {
        printf("user name ....... [1;33m%s[0m\n", pw->pw_name);
        printf("real uid ........ %d\n", uid);
        printf("effective uid ... %d\n", euid);
        printf("\n");
    }


    /* using root permission to setuid(0) */
    printf("Set uid to 0.\n");
    if (setuid(0) != 0)
    {
        perror( "setuid (root)" );
    }
    printf("Who am I?\n");
    system( "whoami" );
    printf("\n");

    printf("Set uid to %d.\n", uid);
    if (setuid( uid ) != 0)
    {
        perror( "setuid (user)" );
    }
    printf("Who am I?\n");
    system( "whoami" );
    printf("\n");


    return 0;
}
