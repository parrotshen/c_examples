#ifndef __BMP_H__
#define __BMP_H__


#define PIXEL_RGB(pixel, r, g, b) \
    { pixel.R = r; pixel.G = g; pixel.B = b; }

typedef struct _tBmpPixel
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
} tBmpPixel;


int bmp8_to_file(
    tBmpPixel *pPalette,
    int        number,
    tBmpPixel *pPixels,
    int        height,
    int        width,
    char      *pName
);

int bmp16_to_file(
    tBmpPixel *pPixels,
    int        height,
    int        width,
    char      *pName
);

int bmp24_to_file(
    tBmpPixel *pPixels,
    int        height,
    int        width,
    char      *pName
);


#endif
