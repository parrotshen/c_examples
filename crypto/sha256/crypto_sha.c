#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "crypto_sha.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

/* x: word; n: number of bits */
#define SHR(x, n)     (((uint32)(x)) >> (n))
#define SHL(x, n)     (((uint32)(x)) << (n))

#define ROTL(x, n)    (((x) << (n)) | ((x) >> (32-(n))))
#define ROTR(x, n)    (((x) >> (n)) | ((x) << (32-(n))))

#define sigma0(x)     (ROTR(x,  7) ^ ROTR(x, 18) ^ SHR(x,  3))
#define sigma1(x)     (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

#define SIGMA0(x)     (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGMA1(x)     (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define CH(x, y, z)   (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z)  (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

static uint8 _padding[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 
       0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 
       0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 
       0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 
       0, 0, 0, 0, 0, 0, 0, 0
};

static uint32 _k[64] = {
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/**
 * Encode word array into byte array.
 * @param  [in]  pOut   Output buffer in bytes.
 * @param  [in]  pIn    Input buffer in words.
 * @param  [in]  inLen  Input length in words.
 */
static void _encode(uint8 *pOut, uint32 *pIn, uint32 inLen)
{
    uint32 i, j;
    uint32 outLen = 4 * inLen; /* bytes */

    for (i = 0, j = 0; i < outLen; i += 4, j++)
    {
        pOut[i    ] = (uint8)((pIn[j] >> 24) & 0xff);
        pOut[i + 1] = (uint8)((pIn[j] >> 16) & 0xff);
        pOut[i + 2] = (uint8)((pIn[j] >>  8) & 0xff);
        pOut[i + 3] = (uint8)((pIn[j]      ) & 0xff);
    }
}

/**
 * Decode byte array into word array.
 * @param  [in]  pOut   Output buffer in words.
 * @param  [in]  pIn    Input buffer in bytes.
 * @param  [in]  inLen  Input length in bytes.
 */
static void _decode(uint32 *pOut, uint8 *pIn, uint32 inLen)
{
    uint32 i, j;
    uint32 outLen = inLen / 4; /* words */

    for (i = 0, j = 0; i < outLen; i++, j += 4)
    {
        pOut[i]  = (uint32)(pIn[j    ] << 24);
        pOut[i] |= (uint32)(pIn[j + 1] << 16);
        pOut[i] |= (uint32)(pIn[j + 2] <<  8);
        pOut[i] |= (uint32)(pIn[j + 3]      );
    }
}

/**
 * SHA-256 block hashing.
 * @param  [in]  pHash   Output hash in words.
 * @param  [in]  pBlock  Input block in bytes.
 */
void _sha256Compute(uint32 *pHash, uint8 *pBlock)
{
    uint8  i;
    uint32 a, b, c, d, e, f, g, h, t, t1, t2;
    uint32 w[64];

    _decode(w, pBlock, 64);

    for (t = 16; t < 64; t++)
    {
        w[t] = sigma1(w[t-2]) + w[t-7] + sigma0(w[t-15]) + w[t-16];
    }

    for (i = 0; i < 16; i++)
    {
        CRYPTO_LOG("W%d = 0x%08x\n", i, w[i]);
    }

    a = pHash[0];
    b = pHash[1];
    c = pHash[2];
    d = pHash[3];
    e = pHash[4];
    f = pHash[5];
    g = pHash[6];
    h = pHash[7];

    for (t = 0; t < 64; t++)
    {
        t1 = h + SIGMA1(e) + CH(e, f, g) + _k[t] + w[t];
        t2 = SIGMA0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
        CRYPTO_LOG("t = %02d: a = 0x%08x, b = 0x%08x, c = 0x%08x, d = 0x%08x\n", t, a, b, c, d);
        CRYPTO_LOG("        e = 0x%08x, f = 0x%08x, g = 0x%08x, h = 0x%08x\n", e, f, g, h);
    }

    pHash[0] = a + pHash[0];
    pHash[1] = b + pHash[1];
    pHash[2] = c + pHash[2];
    pHash[3] = d + pHash[3];
    pHash[4] = e + pHash[4];
    pHash[5] = f + pHash[5];
    pHash[6] = g + pHash[6];
    pHash[7] = h + pHash[7];
}

/**
 * Initialize SHA-256 context.
 * @param  [in]  pCtx  A @ref tCryptoSha256Ctx object.
 */
void crypto_sha256_init(tCryptoSha256Ctx *pCtx)
{
    uint8 bh[32] = {0};

    memset(pCtx, 0, sizeof( tCryptoSha256Ctx ));
    pCtx->h[0] = 0x6a09e667;
    pCtx->h[1] = 0xbb67ae85;
    pCtx->h[2] = 0x3c6ef372;
    pCtx->h[3] = 0xa54ff53a;
    pCtx->h[4] = 0x510e527f;
    pCtx->h[5] = 0x9b05688c;
    pCtx->h[6] = 0x1f83d9ab;
    pCtx->h[7] = 0x5be0cd19;

    _encode(bh, pCtx->h, 8);

    CRYPTO_LOG("H0: \n");
    CRYPTO_DUMP(bh, 32);
}

/**
 * Update a SHA-256 context.
 * @param  [in]  pCtx   A @ref tCryptoSha256Ctx object.
 * @param  [in]  pIn    Input buffer.
 * @param  [in]  inLen  Input length in bytes.
 */
void crypto_sha256_update(tCryptoSha256Ctx *pCtx, uint8 *pIn, uint32 inLen)
{
    uint32 bufLen = ((pCtx->count[1] >> 3) & 0x3f);  /* Length of buffer */
    uint32 remLen = (64 - bufLen);                   /* Length of the remaining buffer */
    uint32 bufIdx = bufLen;                          /* Index of the remaining buffer */
    uint32 inpIdx = 0;                               /* Index of the remaining input */

    /* Update accumulated number of bits of input */
    pCtx->count[1] += (inLen << 3);
    if (pCtx->count[1] < (inLen << 3))
    {
        pCtx->count[0]++;
    }
    pCtx->count[0] += (inLen >> 29);

    /* Compute as possible as times */
    if (inLen >= remLen)
    {
        memcpy(&(pCtx->buffer[bufIdx]), pIn, remLen);
        _sha256Compute(pCtx->h, pCtx->buffer);

        bufIdx = 0;
        inpIdx = remLen;

        while ((inpIdx + 63) < inLen)
        {
            _sha256Compute(pCtx->h, (uint8 *)(&pIn[inpIdx]));
            inpIdx += 64;
        }
    }

    /* Buffer the remaining input */
    memcpy(&(pCtx->buffer[bufIdx]), &pIn[inpIdx], (inLen - inpIdx));
}

/**
 * Final a SHA-256 context.
 * @param  [in]  pCtx   A @ref tCryptoSha256Ctx object.
 * @param  [in]  pOut   Output buffer.
 */
void crypto_sha256_final(tCryptoSha256Ctx *pCtx, uint8 *pOut)
{
    uint32 bufLen;
    uint32 padLen;
    uint8  lenBuf[8];

    /* Save number of bits */
    _encode(lenBuf, pCtx->count, 2);

    /* Append padding */
    bufLen = ((pCtx->count[1] >> 3) & 0x3f);
    padLen = (bufLen < 56) ? (56 - bufLen) : (120 - bufLen);
    crypto_sha256_update(pCtx, _padding, padLen);
    
    /* Append number of bits (before padding) */
    crypto_sha256_update(pCtx, lenBuf, 8);

    /* Store hash values in pOut */
    _encode(pOut, pCtx->h, 8);

    /* Clear context */
    memset(pCtx, 0, sizeof( tCryptoSha256Ctx ));
}

