#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>  /* open */


#define DEV_CONSOLE "/dev/console" /* system console */
#define DEV_TTY     "/dev/tty1"    /* virtual terminal (Ctrl+Alt+Fn) */
#define DEV_UART    "/dev/ttyS0"   /* serial port connection */
#define DEV_PTS     "/dev/pts/0"   /* telnet or ssh connection */


int print_terminal(char *pDev, char *pStr)
{
    int fd;

    fd = open(pDev, O_WRONLY);
    if (fd < 0)
    {
        fprintf(stderr, "ERR: cannot open %s\n", pDev);
        return -1;
    }

    write(fd, pStr, strlen(pStr));

    close( fd );

    return strlen(pStr);
}


int main(int argc, char *argv[])
{
    static char _buf[256];
    int retval;
    int i;

    if (argc < 2)
    {
        snprintf(_buf, 256, "[console] hello\n");
        print_terminal(DEV_CONSOLE, _buf);

        snprintf(_buf, 256, "[tty1   ] hello\n");
        print_terminal(DEV_TTY, _buf);

        snprintf(_buf, 256, "[ttyS0  ] hello\n");
        print_terminal(DEV_UART, _buf);

        snprintf(_buf, 256, "[pts/0  ] hello\n");
        print_terminal(DEV_PTS, _buf);

        return 0;
    }

    for (i=1; i<argc; i++) 
    {
        snprintf(_buf, 256, "[console] %s\n", argv[i]);
        retval = print_terminal(DEV_CONSOLE, _buf);
        if (retval < 0) break;
    }

    for (i=1; i<argc; i++) 
    {
        snprintf(_buf, 256, "[tty1   ] %s\n", argv[i]);
        retval = print_terminal(DEV_TTY, _buf);
        if (retval < 0) break;
    }

    for (i=1; i<argc; i++) 
    {
        snprintf(_buf, 256, "[ttyS0  ] %s\n", argv[i]);
        retval = print_terminal(DEV_UART, _buf);
        if (retval < 0) break;
    }

    for (i=1; i<argc; i++) 
    {
        snprintf(_buf, 256, "[pts/0  ] %s\n", argv[i]);
        retval = print_terminal(DEV_PTS, _buf);
        if (retval < 0) break;
    }

    return 0;
}

