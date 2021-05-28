#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *pArgs[3] = { "/bin/ls", "-al", NULL };

    /* int execve(
    *          const char *filename,
    *          char *const argv[],
    *          char *const envp[]
    *      );
    */
    execve(pArgs[0], pArgs, NULL);
    perror( "execve" );

    return 0;
}
