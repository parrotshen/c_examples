#ifndef __BITMAP_H__
#define __BITMAP_H__


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

/**
 * Set the specific bit in bitmap.
 * @param [in]  map  Bitmap byte array.
 * @param [in]  bit  Bit position.
 */
#define bitmap_set(map, bit) \
    (map[bit >> 3] |= (0x1 << (bit & 0x7)))

/**
 * Clear the specific bit in bitmap.
 * @param [in]  map  Bitmap byte array.
 * @param [in]  bit  Bit position.
 */
#define bitmap_clear(map, bit) \
    (map[bit >> 3] &= ~(0x1 << (bit & 0x7)))

/**
 * Check the specific bit in bitmap.
 * @param [in]  map  Bitmap byte array.
 * @param [in]  bit  Bit position.
 * @returns  Bit set(1) or not(0).
 */
#define bitmap_check(map, bit) \
    ((map[bit >> 3] >> (bit & 0x7)) & 0x1)


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
 * Allocate the bitmap byte array.
 * @param [in]   bits  Bitmap length in bits.
 * @param [out]  pLen  Byte array length.
 * @returns  Byte array.
 */
unsigned char *bitmap_alloc(int bits, int *pLen);

/**
 * Free the bitmap byte array.
 * @param [in]  pBuf  Bitmap byte array.
 */
void bitmap_free(unsigned char *pBuf);


#endif
