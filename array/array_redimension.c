#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ARRAY_REDIMENSION(old, new, a, b) \
    { \
        int i; \
        for (i=0; i<a; i++) \
        { \
            new[i] = (&(old[0][0]) + (i * b)); \
        } \
    }


unsigned char g_array40x8[40][8];


void dump(const void *pMem, unsigned int size)
{
    unsigned char *pByte = (unsigned char *)pMem;
    unsigned int   i;

    if (NULL == pMem)
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
            printf("%p :", (pByte + i));
        }
        if (pByte[i] == 0)
        {
            printf(" [1;31m%2d[0m", pByte[i]);
        }
        else
        {
            printf(" %2d", pByte[i]);
        }
    }
    printf("\n\n");
}

int main(int argc, char *argv[])
{
    unsigned char *pArray4x80[4];
    unsigned char *pArray10x32[10];
    int i;
    int j;


    printf("1. original is a (40 x 8) array:\n");
    for (i=0; i<40; i++)
    {
        for (j=0; j<8; j++)
        {
            g_array40x8[i][j] = j;
        }
    }

    dump(g_array40x8, 320);



    printf("2. try to pretend as a (4 x 80) array:\n");
    for (i=0; i<4; i++)
    {
        for (j=0; j<80; j++)
        {
            g_array40x8[i][j] = j;
        }
    }

    dump(g_array40x8, 320);


    printf("3. re-dimension to a (4 x 80) array:\n");
    #if 0
    ARRAY_REDIMENSION(g_array40x8, pArray4x80, 4, 80);
    #else
    for (i=0; i<4; i++)
    {
        pArray4x80[i] = (&(g_array40x8[0][0]) + (i * 80));
    }
    #endif

    for (i=0; i<4; i++)
    {
        for (j=0; j<80; j++)
        {
            pArray4x80[i][j] = j;
        }
    }

    dump(g_array40x8, 320);


    printf("4. re-dimension to a (10 x 32) array:\n");
    #if 0
    ARRAY_REDIMENSION(g_array40x8, pArray10x32, 10, 32);
    #else
    for (i=0; i<10; i++)
    {
        pArray10x32[i] = (&(g_array40x8[0][0]) + (i * 32));
    }
    #endif

    for (i=0; i<10; i++)
    {
        for (j=0; j<32; j++)
        {
            pArray10x32[i][j] = j;
        }
    }

    dump(g_array40x8, 320);


    return 0;
}

