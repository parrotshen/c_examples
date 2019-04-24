#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void sig_routine(int no)
{
    switch ( no )
    {
        case SIGTERM:
            printf("(%d) SIGTERM\n", no);
            break;
        case SIGINT:
            /* CTrl + C */
            printf("(%d) SIGINT\n", no);
            break;
        case SIGQUIT:
            /* CTrl + \ */
            printf("(%d) SIGQUIT\n", no);
            break;
        case SIGTSTP:
            /* CTrl + Z */
            printf("(%d) SIGTSTP\n", no);
            break;
        case SIGSEGV:
            printf("(%d) SIGSEGV\n", no);
            break;
        case SIGPIPE:
            printf("(%d) SIGPIPE\n", no);
            break;
        case SIGHUP:
            printf("(%d) SIGHUP\n", no);
            break;
        case SIGKILL:
            printf("(%d) SIGKILL\n", no);
            break;
        default:
            printf("(%d) ?\n", no);
    }
}

int main(int argc, char *argv[])
{
    printf("SIGTERM = %2d\n", SIGTERM);
    printf("SIGINT  = %2d\n", SIGINT);
    printf("SIGQUIT = %2d\n", SIGQUIT);
    printf("SIGTSTP = %2d\n", SIGTSTP);
    printf("SIGSEGV = %2d\n", SIGSEGV);
    printf("SIGPIPE = %2d\n", SIGPIPE);
    printf("SIGHUP  = %2d\n", SIGHUP);
    printf("SIGKILL = %2d\n", SIGKILL);
    printf("\n");
    printf("My PID is %d\n", getpid()); 
    printf("\n");

    signal(SIGTERM, sig_routine);
    signal(SIGINT,  sig_routine);
    signal(SIGQUIT, sig_routine);
    signal(SIGTSTP, sig_routine);
    signal(SIGSEGV, sig_routine);
    signal(SIGPIPE, sig_routine);
    signal(SIGHUP,  sig_routine);
    signal(SIGKILL, sig_routine);

    for (;;);

    return 0;
}

