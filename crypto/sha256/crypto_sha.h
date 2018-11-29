#ifndef _CRYPTO_SHA_H_
#define _CRYPTO_SHA_H_

#include "utility.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

/* Length of output message (fixed) */
#define CRYPTO_SHA256_DIGEST_SIZE (32)
#define CRYPTO_SHA256_BLOCK_SIZE  (64)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/* Crypto SHA-256 Context */
typedef struct _tCryptoSha256Ctx
{
    uint32  h[8];       /* Hash values */
    uint32  count[2];   /* Accumulated number of bits of input, < 2^64 (MSB first) */
    uint8   buffer[64]; /*  */
} tCryptoSha256Ctx;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Initialize SHA-256 context.
 * @param  [in]  pCtx  A @ref tCryptoSha256Ctx object.
 */
void crypto_sha256_init(tCryptoSha256Ctx *pCtx);

/**
 * Update a SHA-256 context.
 * @param  [in]  pCtx   A @ref tCryptoSha256Ctx object.
 * @param  [in]  pIn    Input buffer.
 * @param  [in]  inLen  Input length in bytes.
 */
void crypto_sha256_update(tCryptoSha256Ctx *pCtx, uint8 *pIn, uint32 inLen);

/**
 * Final a SHA-256 context.
 * @param  [in]  pCtx   A @ref tCryptoSha256Ctx object.
 * @param  [in]  pOut   Output buffer.
 */
void crypto_sha256_final(tCryptoSha256Ctx *pCtx, uint8 *pOut);


#endif // _CRYPTO_SHA_H_
