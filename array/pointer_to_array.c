#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct _tTest
{
    int  A;
    int  B;
    int  C;
    int  D;
    int  E;
    int  F;
    int  G;
    int  H;
} tTest;


void dump(void *pMem, int size)
{
    unsigned char *pByte = (unsigned char *)pMem;
    int   i;

    if (pMem == NULL)
    {
        printf("NULL\n");
        return;
    }

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }

        if ((i % 16) == 0)
        {
            if ((i % sizeof(tTest)) == 0)
            {
                printf("[1;33m%p[0m :", (pByte + i));
            }
            else
            {
                printf("%p :", (pByte + i));
            }
        }
        printf(" %02x", pByte[i]);
    }
    printf("\n");
    printf("(%d bytes)\n", size);
    printf("\n");
}


int main(void)
{
    tTest *pTest;
    void *pBuf;
    int  *pVal;
    int i;


    pBuf = malloc( 1024 );
    pVal = (int *)pBuf;
    for (i=0; i<256; i++)
    {
        *pVal = (i + 1);
        pVal++;
    }
    printf("pBuf at %p\n", pBuf);
    dump(pBuf, 128);


    pTest = (tTest *)pBuf;

    for (i=0; i<4; i++)
    {
        printf("[1;33mpTest[%d][0m at %p (%d %d %d %d %d %d %d %d)\n",
            i,
            &(pTest[i]),
            pTest[i].A,
            pTest[i].B,
            pTest[i].C,
            pTest[i].D,
            pTest[i].E,
            pTest[i].F,
            pTest[i].G,
            pTest[i].H
        );
    }
    printf("\n");


    free( pBuf );

    return 0;
}

