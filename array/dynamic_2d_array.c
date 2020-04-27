#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void *alloc_2d_array(int m, int n)
{
    int **pArray;
    int  *pElement;
    int   size;
    int   i;


    size = ( (m * sizeof(int *)) + (m * n * sizeof(int)) );

    pArray = (int **)malloc( size );
    memset(pArray, 0, size);

    for (i=0, pElement=(int *)(pArray+m); i<m; i++, pElement+=n)
    {
        pArray[i] = pElement;
    }

    return pArray;
}


int main(int argc, char *argv[])
{
    /* dynamically allocate 2D array with size (m x n) */
    int **pArray;
    int m = 4;
    int n = 10;
    int i;
    int j;


    if (argc > 2)
    {
        m = atoi( argv[1] );
        n = atoi( argv[2] );
    }
    else if (argc > 1)
    {
        m = atoi( argv[1] );
    }


    pArray = alloc_2d_array(m, n);


    /* initialize each element */
    for (i=0; i<m; i++)
    {
        //printf("pArray[[1;35m%d[0m]  %p\n", i, pArray[i]);
        for (j=0; j<n; j++)
        {
            pArray[i][j] = (j + (i * n) + 1);
        }
    }
    printf("\n");


    /* dump array memory */
    {
        unsigned char *pByte = (unsigned char *)pArray;
        int  size;
        int  match;
        int  k, l;

        size = ( (m * sizeof(void *)) + (m * n * sizeof(int)) );

        for (k=0; k<size; k++)
        {
            if ((k != 0) && ((k % 8) == 0))
            {
                printf("\n");
            }

            if ((k % 8) == 0)
            {
                match = 0;
                for (l=0; l<m; l++)
                {
                    if ((void *)(pByte + k) == (void *)pArray[l])
                    {
                        match = 1;
                        break;
                    }
                }
                if ( match )
                {
                    printf("[1;33m%p[0m :", (pByte + k));
                }
                else
                {
                    printf("%p :", (pByte + k));
                }
            }
            if (k < (m * sizeof(void *)))
            {
                printf(" [1;33m%02x[0m", pByte[k]);
            }
            else
            {
                printf(" %02x", pByte[k]);
            }
        }
        printf("\n");
        printf("(%d bytes)\n", size);
        printf("\n");
    }


    /* verify the result */
    printf("pArray[[1;35m%d[0m][[1;35m%d[0m]\n", m, n);
    for (i=0; i<m; i++)
    {
        for (j=0; j<n; j++)
        {
            printf(" %2d", pArray[i][j]);
        }
        printf("\n");
    }
    printf("\n");


    free( pArray );

    return 0;
}

