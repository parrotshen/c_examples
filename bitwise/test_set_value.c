#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitwise.h"


uint8 g_pattern[43][2] = {
    /*
     * [1] 1111 1111
     *     x
     *        7    F
     */
    {  0,  1 },

    /*
     * [2] 1111 1111
     *     xxxx xxx
     *        0    1
     */
    {  0,  7 },

    /*
     * [3] 1111 1111
     *     xxxx xxxx
     *        0    0
     */
    {  0,  8 },

    /*
     * [4] 1111 1111 1111 1111
     *     xxxx xxxx x
     *        0    0    7    F
     */
    {  0,  9 },

    /*
     * [5] 1111 1111 1111 1111
     *     xxxx xxxx xxxx xxx
     *        0    0    0    1
     */
    {  0, 15 },
    /*
     * [6] 1111 1111 1111 1111 1111 1111
     *     xxxx xxxx xxxx xxxx xxxx
     *        0    0    0    0    0    F
     */
    {  0, 20 },

    /*
     * [7] 1111 1111
     *      x
     *        B    F
     */
    {  1,  1 },

    /*
     * [8] 1111 1111
     *      xxx xxxx
     *        8    0
     */
    {  1,  7 },

    /*
     * [9] 1111 1111 1111 1111
     *      xxx xxxx x
     *        8    0    7    F
     */
    {  1,  8 },

    /*
     * [10] 1111 1111 1111 1111
     *       xxx xxxx xx
     *         8    0    3    F
     */
    {  1,  9 },

    /*
     * [11] 1111 1111 1111 1111
     *       xxx xxxx xxxx xxxx
     *         8    0    0    0
     */
    {  1, 15 },

    /*
     * [12] 1111 1111 1111 1111 1111 1111
     *       xxx xxxx xxxx xxxx xxxx x
     *         8    0    0    0    0    7
     */
    {  1, 20 },

    /*
     * [13] 1111 1111
     *              x
     *         F    E
     */
    {  7,  1 },

    /*
     * [14] 1111 1111 1111 1111
     *              x xxxx xx
     *         F    E    0    3
     */
    {  7,  7 },

    /*
     * [15] 1111 1111 1111 1111
     *              x xxxx xxx
     *         F    E    0    1
     */
    {  7,  8 },

    /*
     * [16] 1111 1111 1111 1111
     *              x xxxx xxxx
     *         F    E    0    0
     */
    {  7,  9 },

    /*
     * [17] 1111 1111 1111 1111 1111 1111
     *              x xxxx xxxx xxxx xx
     *         F    E    0    0    0    3
     */
    {  7, 15 },

    /*
     * [18] 1111 1111 1111 1111 1111 1111 1111 1111
     *              x xxxx xxxx xxxx xxxx xxx
     *         F    E    0    0    0    0    1    F
     */
    {  7, 20 },

    /*
     * [19] 1111 1111 1111 1111
     *                x
     *         F    F    7    F
     */
    {  8,  1 },

    /*
     * [20] 1111 1111 1111 1111
     *                xxxx xxx
     *         F    F    0    1
     */
    {  8,  7 },

    /*
     * [21] 1111 1111 1111 1111
     *                xxxx xxxx
     *         F    F    0    0
     */
    {  8,  8 },

    /*
     * [22] 1111 1111 1111 1111 1111 1111
     *                xxxx xxxx x
     *         F    F    0    0    7    F
     */
    {  8,  9 },

    /*
     * [23] 1111 1111 1111 1111 1111 1111
     *                xxxx xxxx xxxx xxx
     *         F    F    0    0    0    1
     */
    {  8, 15 },

    /*
     * [24] 1111 1111 1111 1111 1111 1111 1111 1111
     *                xxxx xxxx xxxx xxxx xxxx
     *         F    F    0    0    0    0    0    F
     */
    {  8, 20 },

    /*
     * [25] 1111 1111 1111 1111
     *                 x
     *         F    F    B    F
     */
    {  9,  1 },

    /*
     * [26] 1111 1111 1111 1111
     *                 xxx xxxx
     *         F    F    8    0
     */
    {  9,  7 },

    /*
     * [27] 1111 1111 1111 1111 1111 1111
     *                 xxx xxxx x
     *         F    F    8    0    7    F
     */
    {  9,  8 },

    /*
     * [28] 1111 1111 1111 1111 1111 1111
     *                 xxx xxxx xx
     *         F    F    8    0    3    F
     */
    {  9,  9 },

    /*
     * [29] 1111 1111 1111 1111 1111 1111
     *                 xxx xxxx xxxx xxxx
     *         F    F    8    0    0    0
     */
    {  9, 15 },

    /*
     * [30] 1111 1111 1111 1111 1111 1111 1111 1111
     *                 xxx xxxx xxxx xxxx xxxx x
     *         F    F    8    0    0    0    0    7
     */
    {  9, 20 },

    /*
     * [31] 1111 1111 1111 1111
     *                        x
     *         F    F    F    E
     */
    { 15,  1 },

    /*
     * [32] 1111 1111 1111 1111 1111 1111
     *                        x xxxx xx
     *         F    F    F    E    0    3
     */
    { 15,  7 },

    /*
     * [33] 1111 1111 1111 1111 1111 1111
     *                        x xxxx xxx
     *         F    F    F    E    0    1
     */
    { 15,  8 },

    /*
     * [34] 1111 1111 1111 1111 1111 1111
     *                        x xxxx xxxx
     *         F    F    F    E    0    0
     */
    { 15,  9 },

    /*
     * [35] 1111 1111 1111 1111 1111 1111 1111 1111
     *                        x xxxx xxxx xxxx xx
     *         F    F    F    E    0    0    0    3
     */
    { 15, 15 },

    /*
     * [36] 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
     *                        x xxxx xxxx xxxx xxxx xxx
     *         F    F    F    E    0    0    0    0    1    F
     */
    { 15, 20 },

    /*
     * [37] 1111 1111 1111 1111 1111 1111
     *                               x
     *         F    F    F    F    F    7
     */
    { 20,  1 },

    /*
     * [38] 1111 1111 1111 1111 1111 1111 1111 1111
     *                               xxxx xxx
     *         F    F    F    F    F    0    1    F
     */
    { 20,  7 },

    /*
     * [39] 1111 1111 1111 1111 1111 1111 1111 1111
     *                               xxxx xxxx
     *         F    F    F    F    F    0    0    F
     */
    { 20,  8 },

    /*
     * [40] 1111 1111 1111 1111 1111 1111 1111 1111
     *                               xxxx xxxx x
     *         F    F    F    F    F    0    0    7
     */
    { 20,  9 },

    /*
     * [41] 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
     *                               xxxx xxxx xxxx xxx
     *         F    F    F    F    F    0    0    0    1    F
     */
    { 20, 15 },

    /*
     * [42] 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
     *                               xxxx xxxx xxxx xxxx xxxx
     *         F    F    F    F    F    0    0    0    0    0
     */
    { 20, 20 },

    /*
     * [43] 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
     *                  xx xxxx xxxx xxxx xxxx xxxx xxxx xxxx xx
     *         F    F    C    0    0    0    0    0    0    0    3
     */
    { 10, 32 }

};

uint8 g_buf[256];


int main(int argc, char *argv[])
{
    int offset;
    int value;
    int bits;
    int i;


    if (argc > 3)
    {
        offset = atoi( argv[1] );
        if ((strlen(argv[2]) > 2) &&
            (argv[2][0] == '0') && (argv[2][1] == 'x'))
        {
            sscanf(argv[2], "0x%x", &value);
        }
        else
        {
            value = atoi( argv[2] );
        }
        bits = atoi( argv[3] );

        if ((offset + bits) > 256)
        {
            printf("ERR: (offset %d + bits %d) > 256\n\n", offset, bits);
            return -1;
        }

        printf("offset %d, value %x, bits %d\n", offset, value, bits);

        memset(g_buf, 0x00, 256);

        bit_set_uint32(g_buf, offset, value, bits);
        bit_dump_color(g_buf, 32, offset, bits);

        return 0;
    }


#if 0
    /*
     * 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
     *                             1 1001 0101 1111 110
     *    F    F    F    F    F    F    9    5    F    B
     */
    offset = 23;
    value = 0xCAFE;
    bits = 16;

    printf("offset %d, value 0x%x, bits %d\n", offset, value, bits);

    memset(g_buf, 0xFF, 256);

    bit_set_uint32(g_buf, offset, value, bits);
    bit_dump_color(g_buf, 8, offset, bits);
    bit_dump(g_buf, 8, 1);
#else
    for (i=0; i<43; i++)
    {
        printf(
            "[%d] offset %d, bits %d\n",
            (i + 1),
            g_pattern[i][0],
            g_pattern[i][1]
        );

        memset(g_buf, 0xFF, 256);

        bit_set_uint32(g_buf, g_pattern[i][0], 0x0, g_pattern[i][1]);
        bit_dump(g_buf, 8, 1);
    }
#endif


    return 0;
}

