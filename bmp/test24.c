#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bmp.h"


int main(int argc, char *argv[])
{
    tBmpPixel bmp[10*3];
    int val = 255;
    int i;

    for (i=0; i<30; i+=3)
    {
        PIXEL_RGB(bmp[i  ], val,   0,   0);
        PIXEL_RGB(bmp[i+1],   0, val,   0);
        PIXEL_RGB(bmp[i+2],   0,   0, val);
        val -= 25;
    }

    bmp24_to_file(bmp, 10, 3, "test24.bmp");

    return 0;
}

