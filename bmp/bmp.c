#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bmp.h"


typedef struct _tBmpFile
{
    /* Bitmap file header (14 bytes) */
    unsigned char  magic[2];
    unsigned int   size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int   offset;

    /* DIP header (40 bytes) */
    unsigned int   dipSize;
    unsigned int   width;
    unsigned int   height;
    unsigned short plane;
    unsigned short pixelBits;
    unsigned int   compress;
    unsigned int   imageSize;
    unsigned int   hResolution;
    unsigned int   vResolution;
    unsigned int   paletteNumber;
    unsigned int   importantColors;
} __attribute__((packed)) tBmpFile;

tBmpFile g_bmp;


/* Bitmap file: 24-bit color */
int bmp_to_file(
    tBmpPixel *pPixels,
    int        width,
    int        height,
    char      *pName
)
{
    FILE *pFile;
    unsigned char pixel[3];
    unsigned char pad = 0;
    int row[2];
    int x;
    int y;
    int i;


    row[0] = (3 * width);
    row[1] = (((24 * width) + 31) / 32) *4;

    g_bmp.magic[0] = 'B';
    g_bmp.magic[1] = 'M';
    g_bmp.size = 54 + (row[1] * height);
    g_bmp.reserved1 = 0;
    g_bmp.reserved2 = 0;
    g_bmp.offset = 54;

    g_bmp.dipSize = 40;
    g_bmp.width = width;
    g_bmp.height = height;
    g_bmp.plane = 1;
    g_bmp.pixelBits = 24;
    g_bmp.compress = 0;
    g_bmp.imageSize = (row[1] * height);
    g_bmp.hResolution = 0;
    g_bmp.vResolution = 0;
    g_bmp.paletteNumber = 0;
    g_bmp.importantColors = 0;


    if ((pFile = fopen(pName, "w")) == NULL)
    {
        perror( "fopen" );
        return -1;
    }

    fwrite(&g_bmp, sizeof(tBmpFile), 1, pFile);

    for (y=(height-1); y>=0; y--)
    {
        i = width * y;
        for (x=0; x<row[0]; x+=3)
        {
            pixel[0] = pPixels[i].B;
            pixel[1] = pPixels[i].G;
            pixel[2] = pPixels[i].R;
            fwrite(pixel, 3, 1, pFile);
            i++;
        }
        for (; x<row[1]; x++)
        {
            fwrite(&pad, 1, 1, pFile);
        }
    }

    fclose( pFile );

    return 0;
}

