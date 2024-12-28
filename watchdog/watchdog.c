#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <errno.h>
#include <linux/watchdog.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define WATCHDOGDEV "/dev/watchdog"


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

int count = 0;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void alarm_handler(int arg)
{
    printf("%d \n", ++count);
    alarm(1);
}

void help(void)
{
    printf("Usage: watchdog [OPTION]...\n");
    printf("\n");
    printf("  -i INTERVAL    Change the watch interval time.\n");
    printf("  -d DEVICE      Assign a watchdog device file.\n");
    printf("  -h             Show this help.\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    int   bootstatus = 0;
    int   interval = 0;
    char *dev = WATCHDOGDEV;
    char  ch;
    int   fd;

    opterr = 0;
    while ((ch=getopt(argc, argv, "i:vh")) != -1)
    {
        switch ( ch )
        {
            case 'i':
                interval = atoi( optarg );
                break;
            case 'd':
                dev = optarg;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    fd = open(dev, O_RDWR);
    if (fd < 0)
    {
        printf("ERR: %s\n\n", strerror(errno));
        return -1;
    }

    signal( SIGALRM, alarm_handler );
    alarm(1);

    if (interval > 0)
    {
        printf("Set watchdog interval to %d sec\n", interval);
        if (ioctl(fd, WDIOC_SETTIMEOUT, &interval) != 0)
        {
            printf("WRN: cannot set watchdog interval\n");
        }
    }

    /* Display current watchdog interval. */
    if (ioctl(fd, WDIOC_GETTIMEOUT, &interval) == 0)
    {
        printf("Current watchdog interval is %d sec\n", interval);
    }
    else
    {
        printf("WRN: cannot get watchdog interval\n");
    }

    /* Check if last boot is caused by watchdog. */
    if (ioctl(fd, WDIOC_GETBOOTSTATUS, &bootstatus) == 0)
    {
        printf(
            "Last boot is caused by '%s'\n",
            ((bootstatus != 0) ? "Watchdog" : "Power-On-Reset")
        );
    }
    else
    {
        printf("WRN: cannot get watchdog status\n");
    }

    printf(
        "Use:\n"
        "  <w> to kick through device node write operation\n"
        "  <i> to kick through IOCTL\n"
        "  <q> to exit the program\n"
        "\n"
    );
    do
    {
        ch = getchar();

        switch (ch)
        {
            case 'w':
                write(fd, "w", 1);
                count = 0;
                printf("Kick watchdog ...\n");
                break;
            case 'i':
                ioctl(fd, WDIOC_KEEPALIVE, NULL);
                count = 0;
                printf("Kick watchdog ...\n");
                break;
            case 'q':
                alarm(0);
                printf("Goodbye !\n");
                break;
            default:
                ;
        }
    } while (ch != 'q');


    /* Write 'V' to close/stop watchdog. */
    write(fd, "V", 1);

    close( fd );

    return 0;
}

