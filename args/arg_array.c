#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int array_1[3][10] = {
  { 100, 101, 102, 103, 104, 105, 106, 107, 108, 109 },
  { 110, 111, 112, 113, 114, 115, 116, 117, 118, 119 },
  { 120, 121, 122, 123, 124, 125, 126, 127, 128, 129 }
};

int array_2[3][10] = {
  { 200, 201, 202, 203, 204, 205, 206, 207, 208, 209 },
  { 210, 211, 212, 213, 214, 215, 216, 217, 218, 219 },
  { 220, 221, 222, 223, 224, 225, 226, 227, 228, 229 }
};

int array_3[3][10] = {
  { 300, 301, 302, 303, 304, 305, 306, 307, 308, 309 },
  { 310, 311, 312, 313, 314, 315, 316, 317, 318, 319 },
  { 320, 321, 322, 323, 324, 325, 326, 327, 328, 329 }
};


void print(int array[3][10])
{
    int i, j; 

    for (i=0; i<3; i++)
    {
        for (j=0; j<10; j++)
        {
            printf(" %d", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


int main(int argc, char *argv[])
{
    int select = 1;

    if (argc > 1) select = atoi( argv[1] );

    switch ( select )
    {
        case 1:
        default:
            print( array_1 );
            break;
        case 2:
            print( array_2 );
            break;
        case 3:
            print( array_3 );
            break;
    }

    return 0;
}

