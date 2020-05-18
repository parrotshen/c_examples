#ifndef __BITWISE_H__
#define __BITWISE_H__


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define uint8  unsigned char
#define uint16 unsigned short
#define uint32 unsigned int
#define uint64 unsigned long long
#define bool   int

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif


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
 * Set 8-bits value to byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  value   Value.
 * @param [in]  bits    Value size in bits.
 */
void bit_set_uint8(uint8 *pBuf, int offset, uint8 value, int bits);

/**
 * Set 16-bits value to byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  value   Value.
 * @param [in]  bits    Value size in bits.
 */
void bit_set_uint16(uint8 *pBuf, int offset, uint16 value, int bits);

/**
 * Set 32-bits value to byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  value   Value.
 * @param [in]  bits    Value size in bits.
 */
void bit_set_uint32(uint8 *pBuf, int offset, uint32 value, int bits);

/**
 * Set 64-bits value to byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  value   Value.
 * @param [in]  bits    Value size in bits.
 */
void bit_set_uint64(uint8 *pBuf, int offset, uint64 value, int bits);


/**
 * Get 8-bits value from byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  bits    Value size in bits.
 * @returns  8-bit value.
 */
uint8 bit_get_uint8(uint8 *pBuf, int offset, int bits);

/**
 * Get 16-bits value from byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  bits    Value size in bits.
 * @returns  16-bit value.
 */
uint16 bit_get_uint16(uint8 *pBuf, int offset, int bits);

/**
 * Get 32-bits value from byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  bits    Value size in bits.
 * @returns  32-bit value.
 */
uint32 bit_get_uint32(uint8 *pBuf, int offset, int bits);

/**
 * Get 64-bits value from byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  bits    Value size in bits.
 * @returns  64-bit value.
 */
uint64 bit_get_uint64(uint8 *pBuf, int offset, int bits);


/**
 * Set data to byte array.
 * @param [in]  pBuf    Buffer.
 * @param [in]  offset  Buffer offset in bits.
 * @param [in]  pData   Data (byte array).
 * @param [in]  bytes   Data size in byte.
 */
void bit_set_data(uint8 *pBuf, int offset, uint8 *pData, int bytes);

/**
 * Get data from byte array.
 * @param [in]   pBuf    Buffer.
 * @param [in]   offset  Buffer offset in bits.
 * @param [out]  pData   Data (byte array).
 * @param [in]   bytes   Data size in bytes.
 */
void bit_get_data(uint8 *pBuf, int offset, uint8 *pData, int bytes);


/**
 * Dump memory in BIT or HEX format.
 * @param [in]  pBuf   Buffer.
 * @param [in]  bytes  Buffer length in bytes.
 * @param [in]  hex    HEX format flag.
 */
void bit_dump(uint8 *pBuf, int bytes, int hex);

/**
 * Dump memory in BIT format with color.
 * @param [in]  pBuf    Buffer.
 * @param [in]  bytes   Buffer length in bytes.
 * @param [in]  offset  Data offset in bits.
 * @param [in]  bits    Data size in bits.
 */
void bit_dump_color(uint8 *pBuf, int bytes, int offset, int bits);


#endif
