#include <stdio.h>
#include <string.h>

/* gets() might exceed the length of buffer */
char g_buffer[80];

int main(int argc, char *argv[])
{
    printf("Key in something:\n");
    gets( g_buffer );

    printf("\n\"%s\"\n", g_buffer);
    printf("(%d bytes)\n", strlen(g_buffer));

    return 0;
}

