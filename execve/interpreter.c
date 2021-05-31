#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i;

    if ((argc > 1) && (0 == strcmp("-verbose", argv[1])))
    {
        for (i=0; i<argc; i++)
        {
            printf("argv[%d]: %s\n", i, argv[i]);
        }
        printf("\n");
    }

    return 0;
}
