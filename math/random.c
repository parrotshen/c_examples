#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


void get_random_bytes(unsigned char *p, int len)
{
    int i;

    srand( (int)time(0) );
    for (i=0; i<len; i++)
    {
        p[i] = (unsigned char)(256.0*rand() / (RAND_MAX));
    }
}

int main(int argc, char *argv[])
{
    unsigned char buffer[1024];
    int length = 0;
    int i;
    int j;

    if (argc != 2)
    {
        printf("Usage: %s <length>\n", argv[0]);
        return -1;
    }

    length = atoi(argv[1]);
    if (length > 1024) length = 1024;

    //srand( (int)time(0) );

    for (j=0; j<3; j++)
    {
        get_random_bytes(buffer, length);
        for (i=0; i<length; i++)
        {
            if ((i != 0) && ((i % 16) == 0))
            {
                printf("\n");
            }
            printf("%02X ", buffer[i]);
        }
        printf("\n\n");

        sleep(1);
    }

    return 0;
}

