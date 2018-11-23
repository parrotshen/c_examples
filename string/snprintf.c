#include <stdio.h>
#include <string.h>

#define TEXT  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/*
 * int snprintf(
 *         char       *str,
 *         size_t      size,
 *         const char *format,
 *         ...
 *     );
 */

int main(void)
{
    char buf[16];
    int n;
    int i;


    for (i=0; i<16; i++)
    {
        buf[i] = ((i < 10) ? i : (16 + i - 10));
    }

    n = snprintf(buf, 4, "%s", TEXT);

    printf("\"%s\"\n", buf);
    printf("%d\n", n);

    printf("\n");
    for (i=0; i<16; i++)
    {
        printf("%02X ", (unsigned char)buf[i]);
    }
    printf("\n\n");

    return 0;
}

