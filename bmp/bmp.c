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


/* Bitmap file: 24-bit color */
int bmp_to_file(
    tBmpPixel *pPixels,
    int        width,
    int        height,
    char      *pName
)
{
    tBmpFile bmpFile;
    FILE *pFile;
    unsigned char pixel[3];
    unsigned char pad = 0;
    int row[2];
    int x;
    int y;
    int i;


    row[0] = (3 * width);
    row[1] = (((24 * width) + 31) / 32) *4;

    bmpFile.magic[0] = 'B';
    bmpFile.magic[1] = 'M';
    bmpFile.size = 54 + (row[1] * height);
    bmpFile.reserved1 = 0;
    bmpFile.reserved2 = 0;
    bmpFile.offset = 54;

    bmpFile.dipSize = 40;
    bmpFile.width = width;
    bmpFile.height = height;
    bmpFile.plane = 1;
    bmpFile.pixelBits = 24;
    bmpFile.compress = 0;
    bmpFile.imageSize = (row[1] * height);
    bmpFile.hResolution = 0;
    bmpFile.vResolution = 0;
    bmpFile.paletteNumber = 0;
    bmpFile.importantColors = 0;


    if ((pFile = fopen(pName, "w")) == NULL)
    {
        perror( "fopen" );
        return -1;
    }

    fwrite(&bmpFile, sizeof(tBmpFile), 1, pFile);

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

