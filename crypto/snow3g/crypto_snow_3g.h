#ifndef _CRYPTO_SNOW_3G_H_
#define _CRYPTO_SNOW_3G_H_

#include "utility.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define crypto_snow_3g_decrypt crypto_snow_3g_encrypt


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/* Structure of SNOW 3G Context */
typedef struct _tCryptoSnow3gCtx {
    /* LFSR */
    uint32 LFSR_S0;
    uint32 LFSR_S1;
    uint32 LFSR_S2;
    uint32 LFSR_S3;
    uint32 LFSR_S4;
    uint32 LFSR_S5;
    uint32 LFSR_S6;
    uint32 LFSR_S7;
    uint32 LFSR_S8;
    uint32 LFSR_S9;
    uint32 LFSR_S10;
    uint32 LFSR_S11;
    uint32 LFSR_S12;
    uint32 LFSR_S13;
    uint32 LFSR_S14;
    uint32 LFSR_S15;
    
    /* FSM */
    uint32 FSM_R1;
    uint32 FSM_R2;
    uint32 FSM_R3;
} tCryptoSnow3gCtx;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * SNOW 3G encryption.
 * @param [in]  pKey    Key value.
 * @param [in]  bearer  Bearer ID.
 * @param [in]  dir     Direction (0: UL, 1: DL).
 * @param [in]  count   Count value.
 * @param [in]  pData   Plaintext to ciphertext.
 * @param [in]  size    Data size in bytes.
 */
void crypto_snow_3g_encrypt(
    uint8  *pKey,
    uint32  bearer,
    uint32  dir,
    uint32  count,
    uint8  *pData,
    uint32  size
);


#endif // _CRYPTO_SNOW_3G_H_
