#include <stdio.h>
#include <string.h>

#define TEXT_1  "Hello World"
#define TEXT_2  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/*
 * char *strncpy(
 *           char   *dest,
 *           char   *src,
 *           size_t  n
 *       );
 */

void dump(char *label, void *addr, int size)
{
    unsigned char *byte = addr;
    int i;

    printf("%s\n", label);
    for (i=0; i<size; i++)
    {
        printf(" %02X", byte[i]);
    }
    printf("\n");
    for (i=0; i<size; i++)
    {
        if (byte[i] == 0x00)
        {
            printf(" \\0");
            break;
        }
        else if (byte[i] == 0xff)
        {
            break;
        }
        else if (byte[i] == 0x0a)
        {
            printf(" \\n");
        }
        else if (byte[i] == 0x0d)
        {
            printf(" \\r");
        }
        else if (byte[i] == 0x20)
        {
            printf(" \\s");
        }
        else
        {
            printf("  %c", (char)byte[i]);
        }
    }
    printf("\n");
    printf("\n");
}

int main(void)
{
    char buf[16];

    memset(buf, 0xff, 16);
    strcpy(buf, TEXT_1);
    dump("strcpy(dst, src)", buf, 16);

    memset(buf, 0xff, 16);
    strncpy(buf, TEXT_1, 5);
    dump("strncpy(dst, src, 5)", buf, 16);

    memset(buf, 0xff, 16);
    strncpy(buf, TEXT_1, 16);
    dump("strncpy(dst, src, 16)", buf, 16);

    memset(buf, 0xff, 16);
    strncpy(buf, TEXT_2, 16);
    dump("strncpy(dst, src, 16)", buf, 16);

    return 0;
}

