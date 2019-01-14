#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define NON_BLOCK_MODE   (1)

#define MAX_BUFFER_SIZE  (2047)
#define SELECT_TOUT_SEC  (0)
#define SELECT_TOUT_USEC (100000)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

unsigned char g_buf[MAX_BUFFER_SIZE+1];
int fd_client = -1;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void mem_dump(const void *addr, unsigned int size)
{
    unsigned char *p = (unsigned char *)addr;
    unsigned int   i;

    if (p == NULL)
    {
        printf("%s: NULL\n", __func__);
        printf("\n");
        return;
    }

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }

        if ((p[i] > 0x1F) && (p[i] < 0x7F))
        {
            printf(" %c ", p[i]);
        }
        else
        {
            printf("%02X ", p[i]);
        }
    }
    printf("\n");
    printf(" (%u bytes)\n", size);
    printf("\n");
}

static void _endHdlr(int arg)
{
    if (fd_client > 0)
    {
        close( fd_client );
        fd_client = -1;
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_in  addr_client;
    socklen_t addr_len_client = sizeof(struct sockaddr_in);

    struct timeval timeout;
    fd_set flags;
    #if (NON_BLOCK_MODE)
    int control;
    #endif
    int retval;
    int size;


    if (argc < 3)
    {
        printf("Usage: nonblock IP_ADDRESS PORT\n");
        printf("\n");
        return 0;
    }

    signal(SIGINT,  _endHdlr);
    signal(SIGKILL, _endHdlr);
    signal(SIGTERM, _endHdlr);


    if ((fd_client=socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror( "socket" );
        return -1;
    }

    /* set non-blocking IO mode */
    #if (NON_BLOCK_MODE)
    control  = fcntl(fd_client, F_GETFL, 0);
    control |= O_NONBLOCK;
    if (fcntl(fd_client, F_SETFL, control) < 0)
    {
        perror( "fcntl" );
        close( fd_client );
        return -1;
    }
    #endif

    /* fill the sockaddr_in structure */
    bzero(&addr_client, addr_len_client);
    addr_client.sin_family      = AF_INET;
    addr_client.sin_port        = htons( atoi(argv[2]) );
    addr_client.sin_addr.s_addr = inet_addr( argv[1] );

    if (connect(fd_client, (struct sockaddr *)&addr_client, addr_len_client) < 0)
    {
        #if (NON_BLOCK_MODE)
        if (EINPROGRESS == errno)
        {
            timeout.tv_sec  = 1;
            timeout.tv_usec = 0;

            FD_ZERO( &flags ); 
            FD_SET(fd_client, &flags);

            if (select(fd_client+1, NULL, &flags, NULL, &timeout) > 0)
            {
                socklen_t len = sizeof( int );
                int val = 0;

                /* get the error status of connection */
                retval = getsockopt(fd_client, SOL_SOCKET, SO_ERROR, &val, &len);
                if (retval != 0)
                {
                    perror( "getsockopt" );
                    close( fd_client );
                    return -1;
                }
                if (val != 0)
                {
                    printf("TCP connect ... failed (%d)\n", val);
                    close( fd_client );
                    return -1;
                }
            }
            else
            {
                perror( "select" );
                close( fd_client );
                return -1;
            }
        }
        else
        #endif
        {
            perror( "connect" );
            close( fd_client );
            return -1;
        }
    }

    printf("TCP connect to %s:%s\n\n", argv[1], argv[2]);

    /* restore the IO mode */
    #if (NON_BLOCK_MODE)
    control &= ~O_NONBLOCK;
    if (fcntl(fd_client, F_SETFL, control) < 0)
    {
        perror( "fcntl" );
        close( fd_client );
        return -1;
    }
    #endif

    timeout.tv_sec  = SELECT_TOUT_SEC;
    timeout.tv_usec = SELECT_TOUT_USEC;

    FD_ZERO( &flags );
    while ( 1 )
    {
        FD_CLR(STDIN_FILENO, &flags);
        FD_SET(STDIN_FILENO, &flags);
        FD_CLR(fd_client, &flags);
        FD_SET(fd_client, &flags);

        retval = select(fd_client+1, &flags, NULL, NULL, &timeout);
        if (retval > 0)
        {
            if ( FD_ISSET(STDIN_FILENO, &flags) )
            {
                size = read(STDIN_FILENO, g_buf, MAX_BUFFER_SIZE);
                if (size > 0)
                {
                    send(fd_client, g_buf, size, 0);
                }
            }

            if ( FD_ISSET(fd_client, &flags) )
            {
                size = recv(fd_client, g_buf, MAX_BUFFER_SIZE, 0);
                if (size > 0)
                {
                    g_buf[size] = 0x00;
                    printf("RECV> %s\n", g_buf);
                }
            }
        }
        else if (retval == 0)
        {
            timeout.tv_sec  = SELECT_TOUT_SEC;
            timeout.tv_usec = SELECT_TOUT_USEC;
        }
        else
        {
            perror( "select" );
            break;
        }
    }

    if (fd_client > 0)
    {
        close( fd_client );
    }

    return 0;
}

