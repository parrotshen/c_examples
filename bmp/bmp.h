#ifndef __BMP_H__
#define __BMP_H__


typedef struct _tBmpPixel
{
    unsigned int R;
    unsigned int G;
    unsigned int B;
} tBmpPixel;


int bmp_to_file(
    tBmpPixel *pPixels,
    int        width,
    int        height,
    char      *pName
);


#endif
