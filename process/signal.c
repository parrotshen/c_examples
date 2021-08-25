#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


void sig_routine(int no)
{
    switch ( no )
    {
        case SIGHUP:
            printf("(%d) SIGHUP\n", no);
            break;
        case SIGINT:
            /* CTrl + C */
            printf("(%d) SIGINT\n", no);
            break;
        case SIGQUIT:
            /* CTrl + \ */
            printf("(%d) SIGQUIT\n", no);
            break;
        case SIGTRAP:
            printf("(%d) SIGTRAP\n", no);
            break;
        case SIGABRT:
            printf("(%d) SIGABRT\n", no);
            break;
        case SIGBUS:
            printf("(%d) SIGBUS\n", no);
            break;
        case SIGFPE:
            printf("(%d) SIGFPE\n", no);
            break;
        case SIGKILL:
            printf("(%d) SIGKILL\n", no);
            break;
        case SIGSEGV:
            printf("(%d) SIGSEGV\n", no);
            break;
        case SIGPIPE:
            printf("(%d) SIGPIPE\n", no);
            break;
        case SIGALRM:
            printf("(%d) SIGALRM\n", no);
            break;
        case SIGTERM:
            printf("(%d) SIGTERM\n", no);
            break;
        case SIGSTOP:
            printf("(%d) SIGSTOP\n", no);
            break;
        case SIGTSTP:
            /* CTrl + Z */
            printf("(%d) SIGTSTP\n", no);
            break;
        case SIGSYS:
            printf("(%d) SIGSYS\n", no);
            break;
        default:
            printf("(%d) ?\n", no);
    }
}

int main(int argc, char *argv[])
{
    char buf[256];
    int size;

    printf("SIGHUP  = %2d\n", SIGHUP);
    printf("SIGINT  = %2d\n", SIGINT);
    printf("SIGQUIT = %2d\n", SIGQUIT);
    printf("SIGTRAP = %2d\n", SIGTRAP);
    printf("SIGABRT = %2d\n", SIGABRT);
    printf("SIGBUS  = %2d\n", SIGBUS);
    printf("SIGFPE  = %2d\n", SIGFPE);
    printf("SIGKILL = %2d\n", SIGKILL);
    printf("SIGSEGV = %2d\n", SIGSEGV);
    printf("SIGPIPE = %2d\n", SIGPIPE);
    printf("SIGALRM = %2d\n", SIGALRM);
    printf("SIGTERM = %2d\n", SIGTERM);
    printf("SIGSTOP = %2d\n", SIGSTOP);
    printf("SIGTSTP = %2d\n", SIGTSTP);
    printf("SIGSYS  = %2d\n", SIGSYS);
    printf("\n");
    printf("My PID is %d\n", getpid()); 
    printf("\n");

    signal(SIGHUP,  sig_routine);
    signal(SIGINT,  sig_routine);
    signal(SIGQUIT, sig_routine);
    signal(SIGTRAP, sig_routine);
    signal(SIGABRT, sig_routine);
    signal(SIGBUS,  sig_routine);
    signal(SIGFPE,  sig_routine);
    signal(SIGKILL, sig_routine);
    signal(SIGSEGV, sig_routine);
    signal(SIGPIPE, sig_routine);
    signal(SIGALRM, sig_routine);
    signal(SIGTERM, sig_routine);
    signal(SIGSTOP, sig_routine);
    signal(SIGTSTP, sig_routine);
    signal(SIGSYS,  sig_routine);

    while (1)
    {
        size = read(STDIN_FILENO, buf, 255);
        buf[size] = '\0';

        if (strlen(buf) > 0)
        {
            buf[strlen(buf) - 1] = '\0';
            if ((0 == strcmp("quit", buf)) ||
                (0 == strcmp("exit", buf)))
            {
                break;
            }
        }
    }

    return 0;
}

