#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    char *nodename = NULL;
    char *servname = NULL;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    unsigned char *byte;
    int cnt = 1;
    int rc;

    if (argc > 2)
    {
        nodename = argv[1];
        servname = argv[1];
    }
    else if (argc > 1)
    {
        nodename = argv[1];
    }
    else
    {
        printf("Usage: getaddrinfo HOST [PORT]\n\n");
        return -1;
    }

    /* Obtain address(es) matching host/port. */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    rc = getaddrinfo(nodename, servname, &hints, &result);
    if (rc != 0)
    {
        printf("ERR: %s\n", gai_strerror(rc));
        return -1;
    }

    /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully connect(2).
       If socket(2) (or connect(2)) fails, we (close the socket
       and) try the next address. */
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        byte = (unsigned char *)(rp->ai_addr);
        if (AF_INET == rp->ai_family)
        {
            printf(
                "#%d IPv4 address %u.%u.%u.%u\n",
                cnt,
                byte[4], byte[5], byte[6], byte[7]
            );
        }
        else if (AF_INET6 == rp->ai_family)
        {
            printf(
                "#%d IPv6 address %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
                cnt,
                byte[0], byte[1], byte[2], byte[3],
                byte[4], byte[5], byte[6], byte[7],
                byte[8], byte[9], byte[10], byte[11],
                byte[12], byte[13], byte[14], byte[15]
            );
        }
        else if (AF_UNSPEC == rp->ai_family)
        {
            printf("#%d Unspecified address\n", cnt);
        }
        else
        {
            printf("#%d Other address (%d)\n", cnt, rp->ai_family);
        }
        #if 0
        printf("ai_flags     0x%x\n", rp->ai_flags);
        printf("ai_family    %d\n", rp->ai_family);
        printf("ai_socktype  %d\n", rp->ai_socktype);
        printf("ai_protocol  %d\n", rp->ai_protocol);
        printf("ai_addrlen   %d\n", rp->ai_addrlen);
        printf("ai_addr      %02x %02x %02x %02x\n", byte[0], byte[1], byte[2], byte[3]);
        printf("             %02x %02x %02x %02x\n", byte[4], byte[5], byte[6], byte[7]);
        printf("             %02x %02x %02x %02x\n", byte[8], byte[9], byte[10], byte[11]);
        printf("             %02x %02x %02x %02x\n", byte[12], byte[13], byte[14], byte[15]);
        printf("ai_canonname %s\n", rp->ai_canonname);
        printf("\n");
        #endif
        cnt++;
    }
    printf("\n");

    freeaddrinfo( result ); /* No longer needed */

    return 0;
}

