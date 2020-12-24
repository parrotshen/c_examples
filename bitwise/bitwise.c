#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitwise.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define DEBUG (0)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

static inline void bit_set_value(uint8 *pBuf, int offset, uint64 value, int bits)
{
    uint8 byte[2];
    uint8 mask;
    int Q[2];
    int R[2];
    int size[2];
    int shift[2];
    int loop;
    int i;
    int j;


    Q[0] = ((offset + bits) >> 3);
    R[0] = ((offset + bits) & 0x7);

    Q[1] = (offset >> 3);
    R[1] = (offset & 0x7);

    loop = (Q[0] + ((R[0] > 0) ? 1 : 0));

    size[0] = 8;    /* bits to set to one byte buffer */
    size[1] = bits; /* bits to process on the next loop */

    for (i=Q[1], j=0; j<loop; i++, j++)
    {
        #if DEBUG
        printf("loop %d:\n", (j + 1));
        #endif

        if ((R[1] + size[1]) > 8)
        {
            size[0] = (8 - R[1]);
            shift[0] = ((R[1] + size[1]) - 8);
            shift[1] = 0;
        }
        else
        {
            size[0] = size[1];
            shift[0] = 0;
            shift[1] = (8 - (R[1] + size[1]));
        }
        mask = ((0x1 << size[0]) - 1);
        byte[0] = (((value >> shift[0]) & mask) << shift[1]);
        #if DEBUG
        printf(" byte[0] mask %02x, size %d\n", mask, size[0]);
        #endif

        if (R[1] > 0)
        {
            mask = (((0x1 << R[1]) - 1) << (8 - R[1]));
        }
        else
        {
            mask = 0;
        }
        if (shift[1] > 0) mask |= ((0x1 << shift[1]) - 1);
        byte[1] = (pBuf[i] & mask);
        #if DEBUG
        printf(" byte[1] mask %02x, size %d\n", mask, R[1]);
        #endif

        pBuf[i] = (byte[0] | byte[1]);
        #if DEBUG
        printf(
            " pBuf[%d] %02x = %02x | %02x\n",
            i,
            pBuf[i],
            byte[0],
            byte[1]
        );
        #endif
        size[1] -= size[0];

        if (0 == j) R[1] = 0;
    }
}

/**
 * Set 8-bits value to byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  value   Value (8-bit).
 * @param [in]  bits    Value size in bits.
 */
void bit_set_uint8(uint8 *pBuf, int offset, uint8 value, int bits)
{
    if (offset < 0)
    {
        printf("ERR: %s ... wrong offset %d\n", __func__, offset);
        return;
    }

    if ((bits <= 0) || (bits > 8))
    {
        printf("ERR: %s ... wrong bits %d\n", __func__, bits);
        return;
    }

    bit_set_value(pBuf, offset, value, bits);
}

/**
 * Set 16-bits value to byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  value   Value (16-bit).
 * @param [in]  bits    Value size in bits.
 */
void bit_set_uint16(uint8 *pBuf, int offset, uint16 value, int bits)
{
    if (offset < 0)
    {
        printf("ERR: %s ... wrong offset %d\n", __func__, offset);
        return;
    }

    if ((bits <= 0) || (bits > 16))
    {
        printf("ERR: %s ... wrong bits %d\n", __func__, bits);
        return;
    }

    bit_set_value(pBuf, offset, value, bits);
}

/**
 * Set 32-bits value to byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  value   Value (32-bit).
 * @param [in]  bits    Value size in bits.
 */
void bit_set_uint32(uint8 *pBuf, int offset, uint32 value, int bits)
{
    if (offset < 0)
    {
        printf("ERR: %s ... wrong offset %d\n", __func__, offset);
        return;
    }

    if ((bits <= 0) || (bits > 32))
    {
        printf("ERR: %s ... wrong bits %d\n", __func__, bits);
        return;
    }

    bit_set_value(pBuf, offset, value, bits);
}

/**
 * Set 64-bits value to byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  value   Value (64-bit).
 * @param [in]  bits    Value size in bits.
 */
void bit_set_uint64(uint8 *pBuf, int offset, uint64 value, int bits)
{
    if (offset < 0)
    {
        printf("ERR: %s ... wrong offset %d\n", __func__, offset);
        return;
    }

    if ((bits <= 0) || (bits > 64))
    {
        printf("ERR: %s ... wrong bits %d\n", __func__, bits);
        return;
    }

    bit_set_value(pBuf, offset, value, bits);
}


static inline uint64 bit_get_value(uint8 *pBuf, int offset, int bits)
{
    uint64 value = 0;
    #if DEBUG
    uint64 temp;
    #endif
    uint8  byte;
    uint8  mask;
    int Q[2];
    int R[2];
    int size[2];
    int shift;
    int loop;
    int i;
    int j;


    Q[0] = ((offset + bits) >> 3);
    R[0] = ((offset + bits) & 0x7);

    Q[1] = (offset >> 3);
    R[1] = (offset & 0x7);

    loop = (Q[0] + ((R[0] > 0) ? 1 : 0));

    size[0] = 8;    /* bits to get from one byte buffer */
    size[1] = bits; /* bits to process on the next loop */

    for (i=Q[1], j=0; j<loop; i++, j++)
    {
        #if DEBUG
        printf("loop %d:\n", (j + 1));
        temp = value;
        #endif

        if ((R[1] + size[1]) > 8)
        {
            size[0] = (8 - R[1]);
            shift = 0;
        }
        else
        {
            size[0] = size[1];
            shift = (8 - (R[1] + size[0]));
        }
        mask = ((0x1 << size[0]) - 1);
        byte = ((pBuf[i] >> shift) & mask);
        #if DEBUG
        printf(" byte %02x, mask %02x, size %d\n", byte, mask, size[0]);
        #endif

        size[1] -= size[0];
        value |= ((uint64)byte << size[1]);
        #if DEBUG
        printf(" value %llx = %llx | %llx\n", value, temp, ((uint64)byte << size[1]));
        #endif

        if (0 == j) R[1] = 0;
    }

    return value;
}

/**
 * Get 8-bits value from byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  bits    Value size in bits.
 * @returns  8-bit value.
 */
uint8 bit_get_uint8(uint8 *pBuf, int offset, int bits)
{
    if (offset < 0)
    {
        printf("ERR: %s ... wrong offset %d\n", __func__, offset);
        return 0;
    }

    if ((bits <= 0) || (bits > 8))
    {
        printf("ERR: %s ... wrong bits %d\n", __func__, bits);
        return 0;
    }

    return bit_get_value(pBuf, offset, bits);
}

/**
 * Get 16-bits value from byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  bits    Value size in bits.
 * @returns  16-bit value.
 */
uint16 bit_get_uint16(uint8 *pBuf, int offset, int bits)
{
    if (offset < 0)
    {
        printf("ERR: %s ... wrong offset %d\n", __func__, offset);
        return 0;
    }

    if ((bits <= 0) || (bits > 16))
    {
        printf("ERR: %s ... wrong bits %d\n", __func__, bits);
        return 0;
    }

    return bit_get_value(pBuf, offset, bits);
}

/**
 * Get 32-bits value from byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  bits    Value size in bits.
 * @returns  32-bit value.
 */
uint32 bit_get_uint32(uint8 *pBuf, int offset, int bits)
{
    if (offset < 0)
    {
        printf("ERR: %s ... wrong offset %d\n", __func__, offset);
        return 0;
    }

    if ((bits <= 0) || (bits > 32))
    {
        printf("ERR: %s ... wrong bits %d\n", __func__, bits);
        return 0;
    }

    return bit_get_value(pBuf, offset, bits);
}

/**
 * Get 64-bits value from byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  bits    Value size in bits.
 * @returns  64-bit value.
 */
uint64 bit_get_uint64(uint8 *pBuf, int offset, int bits)
{
    if (offset < 0)
    {
        printf("ERR: %s ... wrong offset %d\n", __func__, offset);
        return 0;
    }

    if ((bits <= 0) || (bits > 64))
    {
        printf("ERR: %s ... wrong bits %d\n", __func__, bits);
        return 0;
    }

    return bit_get_value(pBuf, offset, bits);
}


/**
 * Set data to byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  pData   Data (byte array).
 * @param [in]  bytes   Data size in byte.
 */
void bit_set_data(uint8 *pBuf, int offset, uint8 *pData, int bytes)
{
    uint8 byte[2];
    uint8 mask;
    int Q[2];
    int R[2];
    int size = 0;
    int shift = 0;
    int loop;
    int i;
    int j;


    if (offset < 0)
    {
        printf("ERR: %s ... wrong offset %d\n", __func__, offset);
        return;
    }

    Q[1] = (offset >> 3);
    R[1] = (offset & 0x7);

    loop = bytes;

    if (R[1] > 0)
    {
        loop++;
        size = (8 - R[1]);
        shift = R[1];

        mask = (((0x1 << R[1]) - 1) << (8 - R[1]));
        byte[1] = (pBuf[ Q[1] ] & mask);
    }

    for (i=Q[1], j=0; j<loop; i++, j++)
    {
        #if DEBUG
        printf("loop %d:\n", (j + 1));
        #endif

        if (R[1] > 0)
        {
            if ((loop - 1) == j)
            {
                mask = ((0x1 << (8 - R[1])) - 1);
                byte[0] = (byte[1] | ((pBuf[i] & mask)));
            }
            else
            {
                mask = ((0x1 << size) - 1);
                byte[0] = ((pData[j] >> shift) & mask);
            }
            #if DEBUG
            printf(" byte[0] mask %02x, size %d\n", mask, size);
            #endif

            pBuf[i] = (byte[0] | byte[1]);
            #if DEBUG
            printf(" pBuf[%d] %02x = %02x | %02x\n", i, pBuf[i], byte[0], byte[1]);
            #endif

            mask = ((0x1 << R[1]) - 1);
            byte[1] = ((pData[j] & mask) << size);
        }
        else
        {
            pBuf[i] = pData[j];
            #if DEBUG
            printf(" pBuf[%d] %02x\n", i, pData[j]);
            #endif
        }
    }
}

/**
 * Get data from byte array.
 * @param [in]   pBuf    Buffer.
 * @param [in]   offset  Buffer offset in bits.
 * @param [out]  pData   Data (byte array).
 * @param [in]   bytes   Data size in bytes.
 */
void bit_get_data(uint8 *pBuf, int offset, uint8 *pData, int bytes)
{
    uint8 byte[2];
    uint8 mask;
    int Q[2];
    int R[2];
    int size = 0;
    int shift = 0;
    int loop;
    int i;
    int j;


    if (offset < 0)
    {
        printf("ERR: %s ... wrong offset %d\n", __func__, offset);
        return;
    }

    Q[1] = (offset >> 3);
    R[1] = (offset & 0x7);

    loop = bytes;

    if (R[1] > 0)
    {
        size = (8 - R[1]);
        shift = R[1];
    }

    for (i=Q[1], j=0; j<loop; i++, j++)
    {
        #if DEBUG
        printf("loop %d:\n", (j + 1));
        #endif

        if (R[1] > 0)
        {
            mask = ((0x1 << size) - 1);
            byte[0] = ((pBuf[i] & mask) << shift);
            #if DEBUG
            printf(" byte[0] mask %02x, size %d\n", mask, size);
            #endif

            mask = ((0x1 << R[1]) - 1);
            byte[1] = ((pBuf[i+1] >> size) & mask);
            #if DEBUG
            printf(" byte[1] mask %02x, size %d\n", mask, R[1]);
            #endif

            pData[j] = (byte[0] | byte[1]);
            #if DEBUG
            printf(" pData[%d] %02x = %02x | %02x\n", j, pData[j], byte[0], byte[1]);
            #endif
        }
        else
        {
            pData[j] = pBuf[i];
            #if DEBUG
            printf(" pData[%d] %02x\n", j, pBuf[i]);
            #endif
        }
    }
}


/**
 * Dump memory in BIT or HEX format.
 * @param [in]  pBuf   Buffer.
 * @param [in]  bytes  Buffer length in bytes.
 * @param [in]  hex    HEX format flag.
 */
void bit_dump(uint8 *pBuf, int bytes, int hex)
{
    int i;

    for (i=0; i<bytes; i++)
    {
        if ( hex )
        {
            if (((i % 8) == 0) && (i != 0)) printf("\n");
            printf(" %02x", pBuf[i]);
        }
        else
        {
            if (((i % 4) == 0) && (i != 0)) printf("\n");
            printf(" %d%d%d%d %d%d%d%d",
                ((pBuf[i] >> 7) & 0x1),
                ((pBuf[i] >> 6) & 0x1),
                ((pBuf[i] >> 5) & 0x1),
                ((pBuf[i] >> 4) & 0x1),
                ((pBuf[i] >> 3) & 0x1),
                ((pBuf[i] >> 2) & 0x1),
                ((pBuf[i] >> 1) & 0x1),
                ((pBuf[i]     ) & 0x1)
            );
        }
    }
    printf("\n\n");
}

/**
 * Dump memory in BIT format with color.
 * @param [in]  pBuf    Buffer.
 * @param [in]  bytes   Buffer length in bytes.
 * @param [in]  offset  Data offset in bits.
 * @param [in]  bits    Data size in bits.
 */
void bit_dump_color(uint8 *pBuf, int bytes, int offset, int bits)
{
    int length;
    int index;
    int j, k;

    length = (bytes << 3);

    for (j=0, k=7; j<length; j++)
    {
        index = (j >> 3);

        if (((j % 32) == 0) && (j != 0)) printf("\n");
        if ((j % 4) == 0) printf(" ");

        if (j == offset) printf("[1;31m");

        printf("%d", ((pBuf[index] >> k--) & 0x1));
        if (k < 0) k = 7;

        if (j == (offset + bits - 1)) printf("[0m");
    }
    printf("\n\n");
}

