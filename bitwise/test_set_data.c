#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitwise.h"


uint8 g_offset[7] = { 0, 1, 7, 8, 9, 15, 20 };
uint8 g_buf[256];


int main(int argc, char *argv[])
{
    uint8 data[2] = { 0x00, 0x00 };
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

    data[0] = 0x2c;
    data[1] = 0xe6;

    bit_set_data(g_buf, 19, data, bytes);
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
        printf(
            "[%d] offset %d, bytes %d\n",
            (i + 1),
            g_offset[i],
            bytes
        );

        memset(g_buf, 0xFF, 256);

        bit_set_data(g_buf, g_offset[i], data, bytes);
        bit_dump(g_buf, 8, 1);
    }
#endif


    return 0;
}

