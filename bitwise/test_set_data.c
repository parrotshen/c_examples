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


    return 0;
}

