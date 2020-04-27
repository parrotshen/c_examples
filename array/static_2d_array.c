#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


unsigned char g_array[4][8] = {
    {  1,  2,  3,  4,  5,  6,  7,  8  },
    { 11, 12, 13, 14, 15, 16, 17, 18  },
    { 21, 22, 23, 24, 25, 26, 27, 28  },
    { 31, 32, 33, 34, 35, 36, 37, 38  }
};


void dump(void *pMem, int size)
{
    unsigned char *pByte = pMem;
    int  i;

    for (i=0; i<size; i++)
    {
        if (0 == i)
        {
            printf("[1;33m%p[0m  %d\n", (pByte + i), pByte[i]);
        }
        else
        {
            printf("%p  %d\n", (pByte + i), pByte[i]);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int i;

    printf("\n");
    for (i=0; i<4; i++)
    {
        printf("g_array[[1;35m%d[0m][...]\n", i);
        dump(g_array[i], sizeof(g_array[i]));
    }
    printf("\n");

    printf("1) array base address:\n");
    printf("\n");
    printf("%p = g_array\n", g_array);
    printf("%p = &g_array\n", &g_array);
    printf("\n\n");
    printf("2) array element address:\n");
    printf("\n");
    printf("%p = &garray[0]\n", &g_array[0]);
    printf("%p = &garray[1]\n", &g_array[1]);
    printf("%p = &garray[2]\n", &g_array[2]);
    printf("%p = &garray[3]\n", &g_array[3]);
    printf("\n");
    printf("%p = &garray[0][0]\n", &g_array[0][0]);
    printf("%p = &garray[1][0]\n", &g_array[1][0]);
    printf("%p = &garray[2][0]\n", &g_array[2][0]);
    printf("%p = &garray[3][0]\n", &g_array[3][0]);
    printf("\n\n");
    printf("3) array element value:\n");
    printf("\n");
    printf("%p = garray[0]\n", g_array[0]);
    printf("%p = garray[1]\n", g_array[1]);
    printf("%p = garray[2]\n", g_array[2]);
    printf("%p = garray[3]\n", g_array[3]);
    printf("\n");
    printf("%2d = garray[0][0]\n", g_array[0][0]);
    printf("%2d = garray[1][0]\n", g_array[1][0]);
    printf("%2d = garray[2][0]\n", g_array[2][0]);
    printf("%2d = garray[3][0]\n", g_array[3][0]);
    printf("\n");
    printf("%p = *(g_array)\n", *(g_array));
    printf("%p = *(g_array + 1)\n", *(g_array + 1));
    printf("%p = *(g_array + 2)\n", *(g_array + 2));
    printf("%p = *(g_array + 3)\n", *(g_array + 3));
    printf("\n");
    printf("%2d = **(g_array)\n", **(g_array));
    printf("%2d = **(g_array + 1)\n", **(g_array + 1));
    printf("%2d = **(g_array + 2)\n", **(g_array + 2));
    printf("%2d = **(g_array + 3)\n", **(g_array + 3));
    printf("\n");
    printf("%2d = *(g_array[0])\n", *(g_array[0]));
    printf("%2d = *(g_array[1])\n", *(g_array[1]));
    printf("%2d = *(g_array[2])\n", *(g_array[2]));
    printf("%2d = *(g_array[3])\n", *(g_array[3]));
    printf("\n\n");


    return 0;
}

