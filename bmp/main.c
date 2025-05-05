#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bmp.h"


int main(int argc, char *argv[])
{
    tBmpPixel bmp[3*10];
    int i;
    int j;

    for (i=0, j=0; i<30; i+=3)
    {
        bmp[i  ].R = 255-j; bmp[i  ].G = 0;     bmp[i  ].B = 0;
        bmp[i+1].R = 0;     bmp[i+1].G = 255-j; bmp[i+1].B = 0;
        bmp[i+2].R = 0;     bmp[i+2].G = 0;     bmp[i+2].B = 255-j;
        j += 20;
    }

    bmp_to_file(bmp, 3, 10, "test.bmp");

    return 0;
}

