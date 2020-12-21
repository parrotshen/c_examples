#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
* Example: 18-bit bitmap array
*
*  array[0]
*   |-- bit7 ......... bitmap[ 7] 
*   |-- bit6 ......... bitmap[ 6] 
*   |-- bit5 ......... bitmap[ 5] 
*   |-- bit4 ......... bitmap[ 4] 
*   |-- bit3 ......... bitmap[ 3] 
*   |-- bit2 ......... bitmap[ 2] 
*   |-- bit1 ......... bitmap[ 1] 
*   `-- bit0 ......... bitmap[ 0]
*
*  array[1]
*   |-- bit7 ......... bitmap[15] 
*   |-- bit6 ......... bitmap[14] 
*   |-- bit5 ......... bitmap[13] 
*   |-- bit4 ......... bitmap[12] 
*   |-- bit3 ......... bitmap[11] 
*   |-- bit2 ......... bitmap[10] 
*   |-- bit1 ......... bitmap[ 9] 
*   `-- bit0 ......... bitmap[ 8]
*
*  array[2]
*   |-- bit7 ......... x 
*   |-- bit6 ......... x 
*   |-- bit5 ......... x 
*   |-- bit4 ......... x 
*   |-- bit3 ......... x 
*   |-- bit2 ......... x 
*   |-- bit1 ......... bitmap[17] 
*   `-- bit0 ......... bitmap[16]
*/


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
 * Allocate the bitmap byte array.
 * @param [in]   bits  Bitmap length in bits.
 * @param [out]  pLen  Byte array length.
 * @returns  Byte array.
 */
unsigned char *bitmap_alloc(int bits, int *pLen)
{
    void *pBuf = NULL;
    int len;

    len = (bits >> 3);
    if (bits & 0x7) len++;
    if ( pLen ) *pLen = len;

    if (len > 0)
    {
        pBuf = malloc( len );
        if ( pBuf )
        {
            memset(pBuf, 0, len);
        }
    }

    return pBuf;
}

/**
 * Free the bitmap byte array.
 * @param [in]  pBuf  Bitmap byte array.
 */
void bitmap_free(unsigned char *pBuf)
{
    if ( pBuf )
    {
        free( pBuf );
    }
}

