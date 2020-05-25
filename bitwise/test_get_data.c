#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitwise.h"


uint8 g_pattern[7][2][8] = {
    { { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      {  0, } },

    { { 0x7f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00 },
      {  1, } },

    { { 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00 },
      {  7, } },

    { { 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 },
      {  8, } },

    { { 0x00, 0x7f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00 },
      {  9, } },

    { { 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00 },
      { 15, } },

    { { 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x00 },
      { 20, } }
};

uint8 g_buf[256];


int main(int argc, char *argv[])
{
    uint8 data[2];
    int bytes = 2;
    int i;


#if 0
    g_buf[0] = 0x88;
    g_buf[1] = 0x0E;
    g_buf[2] = 0x75;
    g_buf[3] = 0x5A;
    g_buf[4] = 0xB1;
    g_buf[5] = 0x84;
    g_buf[6] = 0x02;
    g_buf[7] = 0x03;

    bit_get_data(g_buf, 19, data, bytes);
    printf(
        " offset 19, bytes 2, data 0x%02x%02x\n",
        data[0],
        data[1]
    );
    bit_dump_color(g_buf, 8, 19, 16);
    bit_dump(g_buf, 8, 1);
#else
    for (i=0; i<7; i++)
    {
        memset(data, 0, bytes);
        bit_get_data(
            g_pattern[i][0],
            g_pattern[i][1][0],
            data,
            bytes
        );
        printf(
            "[%d] offset %d, bytes %d, data 0x%02x%02x\n",
            (i + 1),
            g_pattern[i][1][0],
            bytes,
            data[0],
            data[1]
        );
        bit_dump(g_pattern[i][0], 8, 1);
    }
#endif


    return 0;
}

