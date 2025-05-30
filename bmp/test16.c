#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bmp.h"


int main(int argc, char *argv[])
{
    tBmpPixel bmp[10][3];
    int val = 31;
    int i;

    for (i=0; i<10; i++)
    {
        PIXEL_RGB(bmp[i][0], val,   0,   0);
        PIXEL_RGB(bmp[i][1],   0, val,   0);
        PIXEL_RGB(bmp[i][2],   0,   0, val);
        val -= 3;
    }

    bmp16_to_file((tBmpPixel *)bmp, 10, 3, "test16.bmp");

    return 0;
}

