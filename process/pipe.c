#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
*           +--------------+
* read() <==|     pipe     |<== write()
*           +--------------+
*         fd[0]          fd[1]
*/

int main()
{
    int fd_to_child[2];
    int fd_from_child[2];

    pid_t pid_child;

    char  buf[256];
    int   len;


    if (pipe( fd_to_child ) < 0)
    {
        perror( "pipe" );
        exit(-1);
    }

    if (pipe( fd_from_child ) < 0)
    {
        perror( "pipe" );
        exit(-1);
    }

    if ((pid_child = fork()) < 0)
    {
        perror( "fork" );
        exit(-1);
    }

    if (0 == pid_child)
    {
        /* Child process */
        char string1[] = "Child say hello!";
        char string2[] = "Child say goodbye!";

        close( fd_to_child[1] );
        close( fd_from_child[0] );

        printf("[Child ] write pipe\n");
        write(fd_from_child[1], string1, (strlen(string1) + 1));

        len = read(fd_to_child[0], buf, sizeof(buf));
        printf("[Child ] read  pipe\n");
        printf("        \"%s\"\n\n", buf);

        printf("Press ENTER to exit ...\n");
        getchar();

        printf("[Child ] write pipe\n");
        write(fd_from_child[1], string2, (strlen(string2) + 1));

        len = read(fd_to_child[0], buf, sizeof(buf));
        printf("[Child ] read  pipe\n");
        printf("        \"%s\"\n\n", buf);

        close( fd_to_child[0] );
        close( fd_from_child[1] );
        exit(0);
    }
    else
    {
        /* Parent process */
        char string1[] = "Parent say welcome!";
        char string2[] = "Parent say see you!";

        close( fd_to_child[0] );
        close( fd_from_child[1] );

        len = read(fd_from_child[0], buf, sizeof(buf));
        printf("[Parent] read  pipe\n");
        printf("        \"%s\"\n\n", buf);

        printf("[Parent] write pipe\n");
        write(fd_to_child[1], string1, (strlen(string1) + 1));

        len = read(fd_from_child[0], buf, sizeof(buf));
        printf("[Parent] read  pipe\n");
        printf("        \"%s\"\n\n", buf);

        printf("[Parent] write pipe\n");
        write(fd_to_child[1], string2, (strlen(string2) + 1));

        close( fd_to_child[1] );
        close( fd_from_child[0] );
    }

    return 0;
}

