#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int my_system(const char *pCmd)
{
    int status = 0;
    pid_t pid;

    if (NULL == pCmd)
    {
        return -1;
    }

    printf("[system] \"%s\"\n", pCmd);

    if ((pid = fork()) < 0)
    {
        /* fail to fork */
        printf("[system] fork ..... failed\n");
        status = -1;
    }
    else if (pid == 0)
    {
        /* this is the child */
        printf("[system] child .... start\n");

        execl("/bin/sh", "sh", "-c", pCmd, (char *)0);

        printf("[system] child .... exit\n");
        exit(127);
    }
    else
    {
        /* this is the parent */
        printf("[system] parent ... start\n");

        status = 0;
        while (waitpid(pid, &status, 0) < 0)
        {
            if (errno != EINTR)
            {
                status = -1;
                break;
            }
        }

        printf("[system] parent ... exit\n");
    }

    return status;
}

int main(void)
{
    my_system( "/bin/ls -l" );

    return 0;
}

