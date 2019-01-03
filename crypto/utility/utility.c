#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Get bits from a Big Endian byte array.
 * @param [in]  pByte  Byte array.
 * @param [in]  bit    Start bit.
 * @param [in]  num    Number of bits (1 ~ 8).
 * @returns  Bit in one byte.
 */
static uint8 _get_bits_BE(uint8 *pByte, uint32 bit, uint8 num)
{
    uint8 buf = pByte[BIT_ADDRESS(bit)];

    buf >>= (8 - BIT_OFFSET(bit) - num);
    buf  &= (0xFF >> (8 - num));
    return buf;
}

/**
 * Put bits into a Big Endian byte array.
 * @param [in]  pByte  Byte array.
 * @param [in]  bit    Start bit.
 * @param [in]  num    Number of bits (1 ~ 8).
 * @param [in]  value  Bits to put.
 */
static void _put_bits_BE(uint8 *pByte, uint32 bit, uint8 num, uint8 value)
{
    uint8 mask = 0xFF << (8 - num);
    mask = mask >> BIT_OFFSET(bit);

    pByte    += BIT_ADDRESS(bit);
    value   <<= (8 - BIT_OFFSET(bit) - num);
    pByte[0] &= ~mask;
    pByte[0] |= (value & mask);
}

/**
 * Get bits from a Big Endian byte array.
 * @param [in]  pByte  Byte array.
 * @param [in]  bit    Start bit.
 * @param [in]  num    Number of bits (1 ~ 8).
 * @returns  32-bit stream.
 */
uint32 util_getBits(void *pByte, uint32 bit, uint8 num)
{
    uint8 *start = ((uint8 *)pByte) + BIT_ADDRESS(bit);
    uint8 *stop  = ((uint8 *)pByte) + BIT_ADDRESS(bit+num);
    uint8  bitNum = num;
    uint32 tmp    = 0;
    uint32 buf    = 0;

    if ( num == 0 )
    {
        printf( "Get 0 bit!!\n" );
    }
    else if ( num > 32 )
    {
        printf( "Get > 32 bits!!\n" );
    }
    else if ((BIT_OFFSET(bit) + num) <= 8)
    {
        buf = _get_bits_BE((uint8*)pByte, bit, num);
    }
    else if ((BIT_OFFSET(bit) + num) < 40)
    {
        bitNum -= (8 - BIT_OFFSET(bit));
        tmp  = _get_bits_BE(start, BIT_OFFSET(bit), (8 - BIT_OFFSET(bit))) << bitNum;
        buf |= tmp;

        while (start++ < (stop - 1))
        {
            bitNum  -= 8;
            tmp      = *start << bitNum;
            buf     |= tmp;
        }

        bitNum -= BIT_OFFSET(bit+num);
        tmp  = _get_bits_BE(stop, 0, BIT_OFFSET(bit+num)) << bitNum;
        buf |= tmp;
    }

    return buf;
}

/**
 * Put bits into a Big Endian byte array.
 * @param [in]  pByte  Byte array.
 * @param [in]  bit    Start bit.
 * @param [in]  num    Number of bits (1 ~ 8).
 * @param [in]  value  32-bit stream to put.
 */
void util_putBits(void *pByte, uint32 bit, uint8 num, uint32 value)
{
    uint8 *start = ((uint8 *)pByte) + BIT_ADDRESS(bit);
    uint8 *stop  = ((uint8 *)pByte) + BIT_ADDRESS(bit+num);
    uint8  bitNum = num;

    //if (value & (~(0xFFFFFFFF >> (32 - num))))
    //{
    //    printf(
    //        "bit length of value (0x%08X) is greater than (%u), only put lower (%u) bit(s)\n",
    //        value,
    //        num,
    //        num
    //    );
    //}

    value = value & (0xFFFFFFFF >> (32 - num));

    if ( num == 0 )
    {
        printf( "Put 0 bit!!\n" );
    }
    else if ( num > 32 )
    {
        printf( "Put > 32 bits!!\n" );
    }
    else if ((BIT_OFFSET(bit) + num) <= 8)
    {
        _put_bits_BE((uint8 *)pByte, bit, num, (uint8)value);
    }
    else
    {
        bitNum -= (8 - BIT_OFFSET(bit));
        _put_bits_BE(start, BIT_OFFSET(bit), (8 - BIT_OFFSET(bit)), value >> bitNum);

        while (start++ < (stop - 1))
        {
            bitNum -= 8;
            *start = (value >> bitNum) & 0xFF;
        }

        bitNum -= BIT_OFFSET(bit+num);
        _put_bits_BE(stop, 0, BIT_OFFSET(bit+num), value & (0xFF >> (8 - BIT_OFFSET(bit+num))));
    }
}

/**
 * Byte XOR (C = A ^ B).
 * @param [out] pC    Output data.
 * @param [in]  pA    Input data.
 * @param [in]  pB    Input data.
 * @param [in]  size  Input length.
 */
void util_xor(uint8 *pC, uint8 *pA, uint8 *pB, int size)
{
    int i;

    for (i=0; i<size; i++)
    {
        pC[i] = pA[i] ^ pB[i];
    }
}

/**
 * Dump memory.
 * @param [in]  pMem  Memory address.
 * @param [in]  size  size in bytes.
 */
void util_dump(const void *pMem, unsigned int size)
{
    unsigned char *pByte = (unsigned char *)pMem;
    unsigned int   i;

    if (NULL == pMem)
    {
        printf("NULL\n");
        return;
    }

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 8) == 0)) printf("\n");
        printf(" %02X", pByte[i]);
    }
    printf("\n\n");
}

