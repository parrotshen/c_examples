#include <stdio.h>
#include <string.h>

#define TEXT  "Hello World"

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
            printf(" \\n");
            break;
        }
        else if (byte[i] == 0xff)
        {
            break;
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
    strcpy(buf, TEXT);
    dump("strcpy()", buf, 16);

    memset(buf, 0xff, 16);
    strncpy(buf, TEXT, 5);
    dump("strncpy()", buf, 16);

    return 0;
}

