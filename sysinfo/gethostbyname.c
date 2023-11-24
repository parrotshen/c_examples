#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    struct hostent *host = NULL;
    char buf[32];
    int i;

    if (argc != 2)
    {
        printf("Usage: gethostbyname HOST\n\n");
        return -1;
    }

    host = gethostbyname( argv[1] );
    if (NULL == host)
    {
        perror( "gethostbyname" );
        return -1;
    }

    printf("Offical name:\n");
    printf("\t%s\n", host->h_name);
    printf("\n");

    printf("Alias name:\n");
    for (i=0; host->h_aliases[i]; i++)
    {
        printf("\t%s\n", host->h_aliases[i]);
    }
    printf("\n");

    printf("Address list:\n");
    for (i=0; host->h_addr_list[i]; i++)
    {
        printf(
            "\t%s\n",
            inet_ntop(host->h_addrtype, host->h_addr_list[i], buf, (sizeof(buf) - 1))
        );
    }
    printf("\n");

    return 0;
}

