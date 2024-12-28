#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define DUP2_STDOUT 1

int main(int argc, char *argv[])
{
    int oldfd = -1;
    int newfd = -1;
    char *filename = "dup2_stdout_test.txt";
    char buf[256];


    if (argc > 1) filename = argv[1];

    printf("[dup2_stdout] printf at %d\n", __LINE__);
    oldfd = open(filename, O_RDWR);
    if (oldfd >= 0)
    {
        #if (DUP2_STDOUT)
        /* duplicate the opened file to STDOUT */
        newfd = dup2(oldfd, STDOUT_FILENO);
        if (newfd >= 0)
        #endif
        {
            printf("[dup2_stdout] printf at %d\n", __LINE__);
            #if (DUP2_STDOUT)
            close( newfd );
            #endif
        }
        sprintf(buf, "[dup2_stdout] write at %d\n", __LINE__);
        write(oldfd, buf, strlen(buf) + 1);
        close( oldfd );
    }
    printf("[dup2_stdout] printf at %d\n", __LINE__);

    return 0;
}

