#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>  /* open */
#include <syslog.h> /* syslog */


/*
* Check the global system messages:
* $ tail -n 20 /var/log/messages
*/

int main(int argc, char *argv[])
{
    char user[256];
    int i;

    getlogin_r(user, 256);

    openlog(user, LOG_PID, LOG_USER);

    syslog(LOG_INFO, "BEGIN of syslog print test");
    syslog(LOG_INFO, "pid is %d", getpid());
    for (i=1; i<argc; i++)
    {
    syslog(LOG_INFO, "%s", argv[i]);
    }
    syslog(LOG_INFO, "END   of syslog print test");

    closelog();

    return 0;
}

