#ifndef _CRYPTO_AES_H_
#define _CRYPTO_AES_H_

#include "utility.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

/* size in bytes */
#define CRYPTO_AES_128_KEY_SIZE (16)
#define CRYPTO_AES_192_KEY_SIZE (24)
#define CRYPTO_AES_256_KEY_SIZE (32)

#define CRYPTO_AES_MIN_KEY_SIZE CRYPTO_AES_128_KEY_SIZE
#define CRYPTO_AES_MAX_KEY_SIZE CRYPTO_AES_256_KEY_SIZE

#define CRYPTO_AES_BLOCK_SIZE     (16)
#define CRYPTO_AES_BLOCK_NUM(l)   (((l) / CRYPTO_AES_BLOCK_SIZE) + (((l) % CRYPTO_AES_BLOCK_SIZE) ? 1 : 0))
#define CRYPTO_AES_REMAIN_LEN(l)  ((l) % CRYPTO_AES_BLOCK_SIZE)
#define CRYPTO_AES_PADDING_LEN(l) ((CRYPTO_AES_BLOCK_SIZE - ((l) % CRYPTO_AES_BLOCK_SIZE)) % CRYPTO_AES_BLOCK_SIZE)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/* AES Context */
typedef struct _tCryptoAesCtx {
    uint16  keyLen;  /* byte(s) */
    uint32  E[60];
    uint32  D[60];
} tCryptoAesCtx;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Initialize AES.
 */
void crypto_aes_init(void);

/**
 * Set the key for AES context.
 * @param [in]  pCtx    A @ref tCryptoAesCtx object.
 * @param [in]  pKey    Key value.
 * @param [in]  keyLen  Key length (16, 24 or 32).
 * @returns  Success(0) or failure(-1).
 */
int crypto_aes_setKey(tCryptoAesCtx *pCtx, uint8 *pKey, uint16 keyLen);

/**
 * Encrypt a block of data.
 * @param [in]  pCtx  A @ref tCryptoAesCtx object.
 * @param [in]  pOut  Ciphertext buffer.
 * @param [in]  pIn   Plaintext buffer.
 */
void crypto_aes_encrypt(tCryptoAesCtx *pCtx, uint8 *pOut, uint8 *pIn);

/**
 * Decrypt a block of data.
 * @param [in]  pCtx  A @ref tCryptoAesCtx object.
 * @param [in]  pOut  Plaintext buffer.
 * @param [in]  pIn   Ciphertext buffer.
 */
void crypto_aes_decrypt(tCryptoAesCtx *pCtx, uint8 *pOut, uint8 *pIn);

/**
 * Encrypt a sequence of data in CBC mode.
 * @param [in]  pCtx   A @ref tCryptoAesCtx object.
 * @param [in]  pOut   Ciphertext buffer.
 * @param [in]  pIn    Plaintext buffer.
 * @param [in]  inLen  Plaintext length.
 * @param [in]  pIv    IV buffer.
 * @returns  Bytes of the ciphertext.
 */
uint32 crypto_aes_cbc_encrypt(
    tCryptoAesCtx *pCtx,
    uint8         *pOut,
    uint8         *pIn,
    uint32         inLen,
    uint8         *pIv
);

/**
 * Decrypt a sequence of data in CBC mode.
 * @param [in]  pCtx   A @ref tCryptoAesCtx object.
 * @param [in]  pOut   Plaintext buffer.
 * @param [in]  pIn    Ciphertext buffer.
 * @param [in]  inLen  Ciphertext length.
 * @param [in]  pIv    IV buffer.
 * @returns  Bytes of the plaintext.
 */
uint32 crypto_aes_cbc_decrypt(
    tCryptoAesCtx *pCtx,
    uint8         *pOut,
    uint8         *pIn,
    uint32         inLen,
    uint8         *pIv
);


#endif // _CRYPTO_AES_H_
