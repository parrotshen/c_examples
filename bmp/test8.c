#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bmp.h"


int main(int argc, char *argv[])
{
    tBmpPixel pal[30];
    tBmpPixel bmp[10][3];
    int val = 255;
    int i;
    int j;

    for (i=0, j=0; i<10; i++, j+=3)
    {
        PIXEL_RGB(pal[j  ],  val,   0,   0);
        PIXEL_RGB(pal[j+1],    0, val,   0);
        PIXEL_RGB(pal[j+2],    0,   0, val);
        PIXEL_RGB(bmp[i][0], val,   0,   0);
        PIXEL_RGB(bmp[i][1],   0, val,   0);
        PIXEL_RGB(bmp[i][2],   0,   0, val);
        val -= 25;
    }

    bmp8_to_file(pal, 30, (tBmpPixel *)bmp, 10, 3, "test8.bmp");

    return 0;
}

