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


/* Bitmap file: 8-bit color */
int bmp8_to_file(
    tBmpPixel *pPalette,
    int        number,
    tBmpPixel *pPixels,
    int        height,
    int        width,
    char      *pName
)
{
    tBmpFile bmpFile;
    FILE *pFile;
    unsigned int  palette[256];
    unsigned char pixel;
    unsigned char pad = 0;
    int row[2];
    int x;
    int y;
    int i;
    int j;


    row[0] = width;
    row[1] = (((8 * width) + 31) / 32) *4;

    bmpFile.magic[0] = 'B';
    bmpFile.magic[1] = 'M';
    bmpFile.size = 1078 + (row[1] * height);
    bmpFile.reserved1 = 0;
    bmpFile.reserved2 = 0;
    bmpFile.offset = 1078;

    bmpFile.dipSize = 40;
    bmpFile.width = width;
    bmpFile.height = height;
    bmpFile.plane = 1;
    bmpFile.pixelBits = 8;
    bmpFile.compress = 0;
    bmpFile.imageSize = (row[1] * height);
    bmpFile.hResolution = 0;
    bmpFile.vResolution = 0;
    bmpFile.paletteNumber = 0;
    bmpFile.importantColors = 0;

    memset(palette, 0, sizeof(palette));
    for (j=0; j<number && j<256; j++)
    {
        palette[j]
         = (pPalette[j].R << 16) | (pPalette[j].G << 8) | (pPalette[j].B);
    }


    if ((pFile = fopen(pName, "w")) == NULL)
    {
        perror( "fopen" );
        return -1;
    }

    fwrite(&bmpFile, sizeof(tBmpFile), 1, pFile);
    fwrite(palette, sizeof(palette), 1, pFile);

    for (y=(height-1); y>=0; y--)
    {
        i = width * y;
        for (x=0; x<row[0]; x++)
        {
            pixel = 255;
            for (j=0; j<number; j++)
            {
                if ((pPalette[j].R == pPixels[i].R) &&
                    (pPalette[j].G == pPixels[i].G) &&
                    (pPalette[j].B == pPixels[i].B))
                {
                    pixel = j;
                    break;
                }
            }
            fwrite(&pixel, 1, 1, pFile);
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

/* Bitmap file: 16-bit color */
int bmp16_to_file(
    tBmpPixel *pPixels,
    int        height,
    int        width,
    char      *pName
)
{
    tBmpFile bmpFile;
    FILE *pFile;
    unsigned char pixel[2];
    unsigned char pad = 0;
    int row[2];
    int x;
    int y;
    int i;


    row[0] = (2 * width);
    row[1] = (((16 * width) + 31) / 32) *4;

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
    bmpFile.pixelBits = 16;
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
        for (x=0; x<row[0]; x+=2)
        {
            pixel[0] = ((pPixels[i].B & 0x1F)     ) |
                       ((pPixels[i].G & 0x1F) << 5);
            pixel[1] = ((pPixels[i].G & 0x1F) >> 3) |
                       ((pPixels[i].R & 0x1F) << 2);
            fwrite(pixel, 2, 1, pFile);
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

/* Bitmap file: 24-bit color */
int bmp24_to_file(
    tBmpPixel *pPixels,
    int        height,
    int        width,
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

