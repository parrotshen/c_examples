#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> 
#include <termios.h>


struct termios org_opts;
struct termios new_opts;

void routine(int n)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    printf("[alarm] SIGALRM\n"); 
    printf("[alarm] timeout and terminated\n");
    exit(-1);
}

int main(int argc, char *argv[])
{
    int iterations = 10;
    int seconds = 3;


    if (argc > 1)
    {
        seconds = atoi( argv[1] );
    }

    signal(SIGALRM, routine);

    while (iterations-- > 0)
    {
        alarm( seconds );

        printf("[alarm] press any key within %d seconds\n", seconds);
        {
            /* store old settings */
            tcgetattr(STDIN_FILENO, &org_opts);
            /* set new terminal parms */
            memcpy(&new_opts, &org_opts, sizeof( new_opts ));
            new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
            tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
            getchar();
            /* restore old settings */
            tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
        }

        alarm( 0 );
    }

    printf("[alarm] terminated\n");

    return 0;
}

