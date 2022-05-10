#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define NON_BLOCK_MODE 1


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

unsigned char g_buf[256];


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

char *timestamp(void)
{
    static char tstamp[64];
    struct timeval tv;

    gettimeofday(&tv, NULL);
    sprintf(tstamp, "[%lu.%lu]", tv.tv_sec, tv.tv_usec);

    return tstamp;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in  addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    int fd = -1;

    #if (NON_BLOCK_MODE)
    struct timeval timeout;
    fd_set flags;
    int control;
    #endif
    int error;
    int size;


    if (argc < 3)
    {
        printf("Usage: nonblock IP_ADDRESS PORT [-s]\n\n");
        return 0;
    }

    if ((fd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror( "socket" );
        return -1;
    }

    /* set non-blocking IO mode */
    #if (NON_BLOCK_MODE)
    control  = fcntl(fd, F_GETFL, 0);
    control |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, control) < 0)
    {
        perror( "fcntl" );
        close( fd );
        return -1;
    }
    #endif

    /* fill the sockaddr_in structure */
    bzero(&addr, addr_len);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons( atoi(argv[2]) );
    addr.sin_addr.s_addr = inet_addr( argv[1] );

    printf(
        "%s TCP connect to %s:%s\n",
        timestamp(),
        argv[1],
        argv[2]
    );

    if (connect(fd, (struct sockaddr *)&addr, addr_len) < 0)
    {
        #if (NON_BLOCK_MODE)
        if (EINPROGRESS == errno)
        {
            timeout.tv_sec  = 1;
            timeout.tv_usec = 0;

            FD_ZERO( &flags ); 
            FD_SET(fd, &flags);

            printf("%s TCP connect ... wait\n", timestamp());

            if (select(fd+1, NULL, &flags, NULL, &timeout) > 0)
            {
                socklen_t len = sizeof( int );
                int val = 0;

                printf("%s TCP connect ... ready\n", timestamp());

                /* get the error status of connection */
                error = getsockopt(fd, SOL_SOCKET, SO_ERROR, &val, &len);
                if (error != 0)
                {
                    printf("%s TCP connect ... fail\n", timestamp());
                    perror( "getsockopt" );
                    close( fd );
                    return -1;
                }
                if (val != 0)
                {
                    printf("%s TCP connect ... fail (%d)\n", timestamp(), val);
                    close( fd );
                    return -1;
                }
            }
            else
            {
                printf("%s TCP connect ... fail\n", timestamp());
                perror( "select" );
                close( fd );
                return -1;
            }
        }
        else
        #endif
        {
            printf("%s TCP connect ... fail\n", timestamp());
            perror( "connect" );
            close( fd );
            return -1;
        }
    }

    printf("%s TCP connect ... accept\n\n", timestamp());

    if ((argc > 3) && (0 == strcmp("-s", argv[3])))
    {
        sprintf((char *)g_buf, "Hello");
        size = strlen((char *)g_buf);

        printf("-> %s\n", g_buf);
        send(fd, g_buf, size, 0);
    }

    printf("%s TCP receive from server\n", timestamp());

    size = recv(fd, g_buf, 255, 0);
    if (size > 0)
    {
        g_buf[size] = 0x00;
        printf("<- %s\n", (char *)g_buf);
        close( fd );
        return 0;
    }

    printf("%s TCP receive ... fail\n", timestamp());
    perror( "recv" );

    #if (NON_BLOCK_MODE)
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    FD_ZERO( &flags );
    while ( 1 )
    {
        FD_CLR(fd, &flags);
        FD_SET(fd, &flags);

        printf("%s TCP receive ... wait\n", timestamp());

        error = select(fd+1, &flags, NULL, NULL, &timeout);
        if (error > 0)
        {
            printf("%s TCP receive ... ready\n", timestamp());

            if ( FD_ISSET(fd, &flags) )
            {
                size = recv(fd, g_buf, 255, 0);
                if (size > 0)
                {
                    g_buf[size] = 0x00;
                    printf("<- %s\n", (char *)g_buf);
                    close( fd );
                    return 0;
                }

                printf("%s TCP receive ... fail\n", timestamp());
                perror( "recv" );
                close( fd );
                return -1;
            }
        }
        else if (error == 0)
        {
            printf("%s TCP receive ... timeout\n", timestamp());
            timeout.tv_sec  = 1;
            timeout.tv_usec = 0;
        }
        else
        {
            printf("%s TCP receive ... fail\n", timestamp());
            perror( "select" );
            close( fd );
            return -1;
        }
    }
    #endif

    printf("%s TCP disconnect\n", timestamp());
    close( fd );

    return 0;
}

