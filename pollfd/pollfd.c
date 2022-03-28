#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>


unsigned char g_buf[1024];

int openFile(struct pollfd *pPollFd, char *pFile)
{
    int fd;

    memset(pPollFd, 0, sizeof( struct pollfd ));

    fd = open(pFile, (O_RDONLY|O_NONBLOCK));
    if (fd < 0)
    {
        perror( "open" );
        pPollFd->fd = -1;
        return -1;
    }

    pPollFd->fd = fd;
    pPollFd->events = POLLIN;
    pPollFd->revents = 0;

    return 0;
}

void closeFile(struct pollfd *pPollFd)
{
    if (pPollFd->fd > 0)
    {
        close( pPollFd->fd );
        pPollFd->fd = -1;
    }
}

int main(int argc, char *argv[])
{ 
    /*
     * struct pollfd {
     *     int   fd;      // file descriptor
     *     short events;  // requested events
     *     short revents; // returned events
     * };
     */
    struct pollfd pollFd[3];
    int timeout = -1;
    int error;
    int len;


    /* [1] STDIN_FILENO */
    pollFd[0].fd = 0;
    pollFd[0].events = POLLIN;
    pollFd[0].revents = 0;

    /* [2] /dev/random */
    error = openFile(&(pollFd[1]), "/dev/random");
    if (error != 0)
    {
        printf("ERR: fail to open /dev/random\n");
        goto _EXIT;
    }

    /* [3] test.pipe */
    error = openFile(&(pollFd[2]), "test.pipe");
    if (error != 0)
    {
        printf("ERR: fail to open test.pipe\n");
        goto _EXIT;
    }

    for (;;)
    {
        error = poll(pollFd, 3, timeout);

        if (error > 0)
        {
            if (pollFd[0].revents & POLLIN)
            {
                len = read(
                          pollFd[0].fd,
                          g_buf,
                          255
                      );
                if (len < 0)
                {
                    printf("read: %d error\n", pollFd[0].fd);
                    perror( "read" );
                }
                else
                {
                    g_buf[len] = 0;
                    printf("stdin:\n");
                    printf("%s\n", (char *)g_buf);
                    if (0 == strcmp("exit\n", (char *)g_buf)) break;
                }
            }

            if (pollFd[1].revents & POLLIN)
            {
                len = read(
                          pollFd[1].fd,
                          g_buf,
                          16
                      );
                if (len < 0)
                {
                    printf("read: %d error\n", pollFd[1].fd);
                    perror( "read" );
                }
                else
                {
                    printf("/dev/random:\n");
                    printf(
                        " %02x %02x %02x %02x %02x %02x %02x %02x\n",
                        g_buf[0], g_buf[1], g_buf[2], g_buf[3],
                        g_buf[4], g_buf[5], g_buf[6], g_buf[7]
                    );
                    printf(
                        " %02x %02x %02x %02x %02x %02x %02x %02x\n\n",
                        g_buf[8], g_buf[9], g_buf[10], g_buf[11],
                        g_buf[12], g_buf[13], g_buf[14], g_buf[15]
                    );
                }
            }

            if (pollFd[2].revents & POLLIN)
            {
                len = read(
                          pollFd[2].fd,
                          g_buf,
                          255
                      );
                if (len < 0)
                {
                    printf("read: %d error\n", pollFd[2].fd);
                    perror( "read" );
                }
                else
                {
                    g_buf[len] = 0;
                    printf("test.pipe:\n");
                    printf("%s\n", (char *)g_buf);
                }
            }
        }
        else if (error == 0)
        {
            /* system call timeout */
            printf("poll: timeout\n");
        }
        else
        {
            printf("poll: error\n");
            perror( "poll" );
            break;
        }
    }

_EXIT:
    closeFile( &(pollFd[1]) );
    closeFile( &(pollFd[2]) );

    return 0;
}

