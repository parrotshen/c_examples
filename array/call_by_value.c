#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void dump(void *pMem, int size)
{
    unsigned char *pByte = pMem;
    int  i;

    if (NULL == pMem)
    {
        printf("NULL\n");
        return;
    }

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 8) == 0))
        {
            printf("\n");
        }

        if ((i % 8) == 0)
        {
            printf("%p :", (pByte + i));
        }
        printf(" %02X", pByte[i]);
    }
    printf("\n\n");
}

void copy_by_value(void *pBuf, int *pSize)
{
    unsigned char data[] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88,
        0x99, 0xAA, 0xBB, 0xCC,
        0xDD, 0xEE
    };

    printf("2) &pBuf -> %p\n", &pBuf);
    printf("    pBuf -> %p\n", pBuf);
    printf("\n");

    if ( pBuf )
    {
        memcpy(pBuf, data, sizeof(data));
        (*pSize) = sizeof(data);
    }
}

int main(void)
{
    unsigned char buf[1024];
    int size = 0;

    memset(buf, 0, 1024);
    dump(buf, 16);

    printf("1) &buf  -> %p\n", &buf);
    printf("    buf  -> %p\n", buf);
    printf("\n");

    copy_by_value(buf, &size);

    dump(buf, size);

    return 0;
}

