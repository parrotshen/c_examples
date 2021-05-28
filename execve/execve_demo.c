#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2)
    {
        printf("Usage: execve_demo COMMAND [ARGUMENTS...]\n\n");
        return 0;
    }

    printf("execve(");
    for (i=1; i<argc; i++)
    {
        printf("\"%s\", ", argv[i]);
    }
    if (2 == i)
    {
        printf("NULL, ");
    }
    printf("NULL);\n");

    execve(argv[1], &argv[1], NULL);
    perror( "execve" );

    return 0;
}
