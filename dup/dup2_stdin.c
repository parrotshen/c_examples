#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define DUP2_STDIN 1

int main(int argc, char *argv[])
{
    int oldfd = -1;
    int newfd = -1;
    char *filename = "dup2_stdin_test.txt";
    char buf[256];
    int i;


    if (argc > 1) filename = argv[1];

    oldfd = open(filename, O_RDONLY);
    if (oldfd >= 0)
    {
        #if (DUP2_STDIN)
        /* duplicate the opened file to STDIN */
        newfd = dup2(oldfd, STDIN_FILENO);
        if (newfd >= 0)
        #endif
        {
            for (i=0; i<4; i++)
            {
                buf[0] = 0;
                if ( gets( buf ) ) printf("[dup2_stdin] %s\n", buf);
            }
            #if (DUP2_STDIN)
            close( newfd );
            #endif
        }
        close( oldfd );
    }

    return 0;
}

