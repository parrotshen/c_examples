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

void copy_by_reference(unsigned char **pAddr, int *pSize)
{
    unsigned char data[] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88,
        0x99, 0xAA, 0xBB, 0xCC,
        0xDD, 0xEE
    };

    printf("2) &pAddr -> %p\n", &pAddr);
    printf("    pAddr -> %p\n", pAddr);
    printf("   *pAddr -> %p\n", *pAddr);
    printf("\n");

    (*pAddr) = malloc( sizeof(data) );

    memcpy((*pAddr), data, sizeof(data));

    (*pSize) = sizeof(data);

    printf("3)  pAddr -> %p\n", pAddr);
    printf("   *pAddr -> %p\n", *pAddr);
    printf("\n");
}

int main(void)
{
    unsigned char *pBuf = NULL;
    int size = 0;

    printf("1)  &pBuf -> %p\n", &pBuf);
    printf("     pBuf -> %p\n", pBuf);
    printf("\n");

    copy_by_reference(&pBuf, &size);

    printf("4)   pBuf -> %p\n", pBuf);
    printf("\n");

    if ( pBuf )
    {
        dump(pBuf, size);
        free( pBuf );
    }

    return 0;
}

