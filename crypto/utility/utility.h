#ifndef _UTILITY_H_
#define _UTILITY_H_


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

/* NULL value */
#undef  NULL
#define NULL  ((void *)0)


/* byte order */
#if defined(_LITTLE_ENDIAN_)

# define le32_to_cpu(v)  (v)
# define cpu_to_le32(v)  (v)

#elif defined(_BIG_ENDIAN_)

# define le32_to_cpu(v)  ((((v) & 0x000000FF) << 24) | \
                          (((v) & 0x0000FF00) <<  8) | \
                          (((v) & 0x00FF0000) >>  8) | \
                          (((v) & 0xFF000000) >> 24))
# define cpu_to_le32(v)  le32_to_cpu(v)

#endif // _LITTLE_ENDIAN_


/**
 * Convert a byte array to an uint16 integer.
 * @param [in]   pBuf  Byte array.
 * @param [out]  val   2-byte integer.
 */
#define BYTE_ARRAY_TO_UINT16(pBuf, val) \
    do { \
        val  = (*((uint8 *)pBuf  ) <<  8); \
        val |= (*((uint8 *)pBuf+1)      ); \
    } while(0)

/**
 * Convert an uint16 integer to a byte array.
 * @param [in]   val   2-byte integer.
 * @param [out]  pBuf  Byte array.
 */
#define UINT16_TO_BYTE_ARRAY(val, pBuf) \
    do { \
        *((uint8 *)pBuf  ) = ((val >>  8) & 0xFF); \
        *((uint8 *)pBuf+1) = ((val      ) & 0xFF); \
    } while (0)

/**
 * Convert a byte array to an uint32 integer.
 * @param [in]   pBuf  Byte array.
 * @param [out]  val   4-byte integer.
 */
#define BYTE_ARRAY_TO_UINT32(pBuf, val) \
    do { \
        val  = (*((uint8 *)pBuf  ) << 24); \
        val |= (*((uint8 *)pBuf+1) << 16); \
        val |= (*((uint8 *)pBuf+2) <<  8); \
        val |= (*((uint8 *)pBuf+3)      ); \
    } while (0)

/**
 * Convert an uint32 integer to a byte array.
 * @param [in]   val   4-byte integer.
 * @param [out]  pBuf  Byte array.
 */
#define UINT32_TO_BYTE_ARRAY(val, pBuf) \
    do { \
        *((uint8 *)pBuf  ) = ((val >> 24) & 0xFF); \
        *((uint8 *)pBuf+1) = ((val >> 16) & 0xFF); \
        *((uint8 *)pBuf+2) = ((val >>  8) & 0xFF); \
        *((uint8 *)pBuf+3) = ((val      ) & 0xFF); \
    } while (0)

/**
 * Convert a byte array to an uint64 integer.
 * @param [in]   pBuf  Byte array.
 * @param [out]  val   8-byte integer.
 */
#define BYTE_ARRAY_TO_UINT64(pBuf, val) \
    do { \
        val  = ((uint64)(*((uint8 *)pBuf  )) << 56); \
        val |= ((uint64)(*((uint8 *)pBuf+1)) << 48); \
        val |= ((uint64)(*((uint8 *)pBuf+2)) << 40); \
        val |= ((uint64)(*((uint8 *)pBuf+3)) << 32); \
        val |= ((uint64)(*((uint8 *)pBuf+4)) << 24); \
        val |= ((uint64)(*((uint8 *)pBuf+5)) << 16); \
        val |= ((uint64)(*((uint8 *)pBuf+6)) <<  8); \
        val |= ((uint64)(*((uint8 *)pBuf+7))      ); \
    } while (0)

/**
 * Convert an uint64 integer to a byte array.
 * @param [in]   val   8-byte integer.
 * @param [out]  pBuf  Byte array.
 */
#define UINT64_TO_BYTE_ARRAY(val, pBuf) \
    do { \
        *((uint8 *)pBuf  ) = ((val >> 56) & 0xFF); \
        *((uint8 *)pBuf+1) = ((val >> 48) & 0xFF); \
        *((uint8 *)pBuf+2) = ((val >> 40) & 0xFF); \
        *((uint8 *)pBuf+3) = ((val >> 32) & 0xFF); \
        *((uint8 *)pBuf+4) = ((val >> 24) & 0xFF); \
        *((uint8 *)pBuf+5) = ((val >> 16) & 0xFF); \
        *((uint8 *)pBuf+6) = ((val >>  8) & 0xFF); \
        *((uint8 *)pBuf+7) = ((val      ) & 0xFF); \
    } while (0)


/* Convert the position of bit to byte */
#define BIT_ADDRESS(b)  ((b) >> 3)

/**
 * Get the number of bits less than a byte.
 * @param [in]  b  Number of bits.
 * @returns  Number of bits.
 */
#define BIT_OFFSET(b)  ((b) % 8)

/**
 * Get the numbers of lack of bit in a group.
 * @param [in]  b  Number of bits.
 * @param [in]  n  Number of bits in a group.
 * @returns  Number of bits.
 */
#define BIT_PADDING(b, n)  (((b) % (n)) ? ((n) - ((b) % (n))) : 0)

/**
 * Convert number of bits to number of bytes.
 * @param [in]  n  Number of bits.
 * @returns  Number of bytes.
 */
#define BIT_TO_BYTE(n)  ((n) >> 3)

/**
 * Convert number of bytes to number of bits.
 * @param [in]  n  Number of bytes.
 * @returns  Number of bits.
 */
#define BYTE_TO_BIT(n)  ((n) << 3)


#define BYTE_ARRAY_GET_BITS(p, s, n)     util_getBits(p, s, n)
#define BYTE_ARRAY_PUT_BITS(p, s, n, v)  util_putBits(p, s, n, v)


/* log for debug */
#if 0
# define CRYPTO_LOG   printf
# define CRYPTO_DUMP  util_dump
#else
# define CRYPTO_LOG( a... )
# define CRYPTO_DUMP( a... )
#endif


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/* Integers */
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef unsigned long long  uint64;

typedef signed char     int8;
typedef signed short    int16;
typedef signed int      int32;
typedef signed long long    int64;

/*
*  On 32-bit machine, sizeof( long ) is 4
*  On 64-bit machine, sizeof( long ) is 8
*  Using %lu to print intptr
*/
typedef unsigned long   intptr;


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
 * @returns  32-bit stream.
 */
uint32 util_getBits(void *pByte, uint32 bit, uint8 num);

/**
 * Put bits into a Big Endian byte array.
 * @param [in]  pByte  Byte array.
 * @param [in]  bit    Start bit.
 * @param [in]  num    Number of bits (1 ~ 8).
 * @param [in]  value  32-bit stream to put.
 */
void util_putBits(void *pByte, uint32 bit, uint8 num, uint32 value);

/**
 * Byte XOR (C = A ^ B).
 * @param [out] pC    Output data.
 * @param [in]  pA    Input data.
 * @param [in]  pB    Input data.
 * @param [in]  size  Input length.
 */
void util_xor(uint8 *pC, uint8 *pA, uint8 *pB, int size);

/**
 * Dump memory.
 * @param [in]  pMem  Memory address.
 * @param [in]  size  size in bytes.
 */
void util_dump(const void *pMem, unsigned int size);


#endif // _UTILITY_H_
