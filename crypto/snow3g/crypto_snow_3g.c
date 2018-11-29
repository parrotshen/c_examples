#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "crypto_snow_3g.h"

/* Some changes from the Gladman version:
    s/RIJNDAEL(e_key)/E_KEY/g
    s/RIJNDAEL(d_key)/D_KEY/g
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

/* Rijndael S-box SR */
uint8 SR[256] = {
0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,
0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,
0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,
0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,
0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,
0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,
0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,
0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB,
0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,
0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,
0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,
0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,
0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,
0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16 }; 

/* S-box SQ */
uint8 SQ[256] = {
0x25,0x24,0x73,0x67,0xD7,0xAE,0x5C,0x30,0xA4,0xEE,0x6E,0xCB,0x7D,0xB5,0x82,0xDB,
0xE4,0x8E,0x48,0x49,0x4F,0x5D,0x6A,0x78,0x70,0x88,0xE8,0x5F,0x5E,0x84,0x65,0xE2,
0xD8,0xE9,0xCC,0xED,0x40,0x2F,0x11,0x28,0x57,0xD2,0xAC,0xE3,0x4A,0x15,0x1B,0xB9,
0xB2,0x80,0x85,0xA6,0x2E,0x02,0x47,0x29,0x07,0x4B,0x0E,0xC1,0x51,0xAA,0x89,0xD4,
0xCA,0x01,0x46,0xB3,0xEF,0xDD,0x44,0x7B,0xC2,0x7F,0xBE,0xC3,0x9F,0x20,0x4C,0x64,
0x83,0xA2,0x68,0x42,0x13,0xB4,0x41,0xCD,0xBA,0xC6,0xBB,0x6D,0x4D,0x71,0x21,0xF4,
0x8D,0xB0,0xE5,0x93,0xFE,0x8F,0xE6,0xCF,0x43,0x45,0x31,0x22,0x37,0x36,0x96,0xFA,
0xBC,0x0F,0x08,0x52,0x1D,0x55,0x1A,0xC5,0x4E,0x23,0x69,0x7A,0x92,0xFF,0x5B,0x5A,
0xEB,0x9A,0x1C,0xA9,0xD1,0x7E,0x0D,0xFC,0x50,0x8A,0xB6,0x62,0xF5,0x0A,0xF8,0xDC,
0x03,0x3C,0x0C,0x39,0xF1,0xB8,0xF3,0x3D,0xF2,0xD5,0x97,0x66,0x81,0x32,0xA0,0x00,
0x06,0xCE,0xF6,0xEA,0xB7,0x17,0xF7,0x8C,0x79,0xD6,0xA7,0xBF,0x8B,0x3F,0x1F,0x53,
0x63,0x75,0x35,0x2C,0x60,0xFD,0x27,0xD3,0x94,0xA5,0x7C,0xA1,0x05,0x58,0x2D,0xBD,
0xD9,0xC7,0xAF,0x6B,0x54,0x0B,0xE0,0x38,0x04,0xC8,0x9D,0xE7,0x14,0xB1,0x87,0x9C,
0xDF,0x6F,0xF9,0xDA,0x2A,0xC4,0x59,0x16,0x74,0x91,0xAB,0x26,0x61,0x76,0x34,0x2B,
0xAD,0x99,0xFB,0x72,0xEC,0x33,0x12,0xDE,0x98,0x3B,0xC0,0x9B,0x3E,0x18,0x10,0x3A,
0x56,0xE1,0x77,0xC9,0x1E,0x9E,0x95,0xA3,0x90,0x19,0xA8,0x6C,0x09,0xD0,0xF0,0x86 };


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

/* MULx.
 * Input V: an 8-bit input.
 * Input c: an 8-bit input.
 * Output : an 8-bit output.
 * See section 3.1.1 for details.
 */
uint8 MULx(uint8 V, uint8 c)
{
    if ( V & 0x80 ) 
        return ( (V << 1) ^ c);
    else 
        return ( V << 1);
}

/* MULxPOW.
 * Input V: an 8-bit input.
 * Input i: a positive integer.
 * Input c: an 8-bit input.
 * Output : an 8-bit output.
 * See section 3.1.2 for details.
 */
uint8 MULxPOW(uint8 V, uint8 i, uint8 c)
{
    if ( i == 0)
        return V;
    else
        return MULx( MULxPOW( V, i-1, c ), c);
}

/* The function MUL alpha.
 * Input c: 8-bit input.
 * Output : 32-bit output.
 * See section 3.4.2 for details.
 */
uint32 MULalpha(uint8 c)
{
   return ( ( ((uint32)MULxPOW(c,  23, 0xa9)) << 24 ) |  
            ( ((uint32)MULxPOW(c, 245, 0xa9)) << 16 ) |  
            ( ((uint32)MULxPOW(c,  48, 0xa9)) <<  8 ) |  
            ( ((uint32)MULxPOW(c, 239, 0xa9))       ) );
}

/* The function DIV alpha.
 * Input c: 8-bit input.
 * Output : 32-bit output.
 * See section 3.4.3 for details.
 */
uint32 DIValpha(uint8 c)
{
   return ( ( ((uint32)MULxPOW(c,  16, 0xa9)) << 24 ) |  
            ( ((uint32)MULxPOW(c,  39, 0xa9)) << 16 ) |  
            ( ((uint32)MULxPOW(c,   6, 0xa9)) <<  8 ) |  
            ( ((uint32)MULxPOW(c,  64, 0xa9))       ) );
}

/* The 32x32-bit S-Box S1
 * Input: a 32-bit input.
 * Output: a 32-bit output of S1 box.
 * See section 3.3.1. 
 */
uint32 S1(uint32 w)
{
    uint8 r0=0, r1=0, r2=0, r3=0;
    uint8 srw0 = SR[ (uint8)((w >> 24) & 0xff) ];
    uint8 srw1 = SR[ (uint8)((w >> 16) & 0xff) ];
    uint8 srw2 = SR[ (uint8)((w >>  8) & 0xff) ];
    uint8 srw3 = SR[ (uint8)((w)       & 0xff) ];

    r0 = ( ( MULx( srw0 , 0x1b) )  ^
           ( srw1 ) ^
           ( srw2 ) ^
           ( (MULx( srw3, 0x1b)) ^ srw3 ) );

    r1 = ( ( ( MULx( srw0 , 0x1b) ) ^ srw0 ) ^
           ( MULx(srw1, 0x1b) ) ^
           ( srw2 ) ^
           ( srw3 ) );

    r2 = ( ( srw0 ) ^
           ( ( MULx( srw1 , 0x1b) ) ^ srw1 ) ^
           ( MULx(srw2, 0x1b) ) ^
           ( srw3 ) );

    r3 = ( ( srw0 ) ^
           ( srw1 ) ^
           ( ( MULx( srw2 , 0x1b) ) ^ srw2 ) ^
           ( MULx( srw3, 0x1b) ) );

    return ( ( ((uint32)r0) << 24 ) |
             ( ((uint32)r1) << 16 ) |
             ( ((uint32)r2) <<  8 ) |
             ( ((uint32)r3)       ) );
}

/* The 32x32-bit S-Box S2
 * Input: a 32-bit input.
 * Output: a 32-bit output of S2 box.
 * See section 3.3.2.
 */
uint32 S2(uint32 w)
{
    uint8 r0=0, r1=0, r2=0, r3=0;
    uint8 sqw0 = SQ[ (uint8)((w >> 24) & 0xff) ];
    uint8 sqw1 = SQ[ (uint8)((w >> 16) & 0xff) ];
    uint8 sqw2 = SQ[ (uint8)((w >>  8) & 0xff) ];
    uint8 sqw3 = SQ[ (uint8)((w)       & 0xff) ];

    r0 = ( ( MULx( sqw0 , 0x69) )  ^
           ( sqw1 ) ^
           ( sqw2 ) ^
           ( (MULx( sqw3, 0x69)) ^ sqw3 ) );

    r1 = ( ( ( MULx( sqw0 , 0x69) ) ^ sqw0 ) ^
           ( MULx(sqw1, 0x69) ) ^
           ( sqw2 ) ^
           ( sqw3 ) );

    r2 = ( ( sqw0 ) ^
           ( ( MULx( sqw1 , 0x69) ) ^ sqw1 ) ^
           ( MULx(sqw2, 0x69) ) ^
           ( sqw3 ) );

    r3 = ( ( sqw0 ) ^
           ( sqw1 ) ^
           ( ( MULx( sqw2 , 0x69) ) ^ sqw2 ) ^
           ( MULx( sqw3, 0x69) ) );

    return ( ( ((uint32)r0) << 24 ) |
             ( ((uint32)r1) << 16 ) |
             ( ((uint32)r2) <<  8 ) |
             ( ((uint32)r3)       ) );
}

/* Clocking LFSR in initialization mode.
 * LFSR Registers S0 to S15 are updated as the LFSR receives a single clock.
 * Input F: a 32-bit word comes from output of FSM.
 * See section 3.4.4.
 */ 
void ClockLFSRInitializationMode(tCryptoSnow3gCtx *pCtx, uint32 F)
{
    uint32 v = ( ( (pCtx->LFSR_S0 << 8) & 0xffffff00 ) ^
                 ( MULalpha( (uint8)((pCtx->LFSR_S0>>24) & 0xff) ) ) ^
                 ( pCtx->LFSR_S2 ) ^
                 ( (pCtx->LFSR_S11 >> 8)  & 0x00ffffff ) ^
                 ( DIValpha( (uint8)( ( pCtx->LFSR_S11) & 0xff ) ) ) ^
                 ( F ) );
    pCtx->LFSR_S0 = pCtx->LFSR_S1;
    pCtx->LFSR_S1 = pCtx->LFSR_S2;
    pCtx->LFSR_S2 = pCtx->LFSR_S3;
    pCtx->LFSR_S3 = pCtx->LFSR_S4;
    pCtx->LFSR_S4 = pCtx->LFSR_S5;
    pCtx->LFSR_S5 = pCtx->LFSR_S6;
    pCtx->LFSR_S6 = pCtx->LFSR_S7;
    pCtx->LFSR_S7 = pCtx->LFSR_S8;
    pCtx->LFSR_S8 = pCtx->LFSR_S9;
    pCtx->LFSR_S9 = pCtx->LFSR_S10;
    pCtx->LFSR_S10 = pCtx->LFSR_S11;
    pCtx->LFSR_S11 = pCtx->LFSR_S12;
    pCtx->LFSR_S12 = pCtx->LFSR_S13;
    pCtx->LFSR_S13 = pCtx->LFSR_S14;
    pCtx->LFSR_S14 = pCtx->LFSR_S15;
    pCtx->LFSR_S15 = v;
} 

/* Clocking LFSR in keystream mode.
 * LFSR Registers S0 to S15 are updated as the LFSR receives a single clock.
 * See section 3.4.5.
 */ 
void ClockLFSRKeyStreamMode(tCryptoSnow3gCtx *pCtx)
{
    uint32 v = ( ( (pCtx->LFSR_S0 << 8) & 0xffffff00 ) ^
                 ( MULalpha( (uint8)((pCtx->LFSR_S0>>24) & 0xff) ) ) ^
                 ( pCtx->LFSR_S2 ) ^
                 ( (pCtx->LFSR_S11 >> 8)  & 0x00ffffff ) ^
                 ( DIValpha( (uint8)( ( pCtx->LFSR_S11) & 0xff ) ) ) );
    pCtx->LFSR_S0 = pCtx->LFSR_S1;
    pCtx->LFSR_S1 = pCtx->LFSR_S2;
    pCtx->LFSR_S2 = pCtx->LFSR_S3;
    pCtx->LFSR_S3 = pCtx->LFSR_S4;
    pCtx->LFSR_S4 = pCtx->LFSR_S5;
    pCtx->LFSR_S5 = pCtx->LFSR_S6;
    pCtx->LFSR_S6 = pCtx->LFSR_S7;
    pCtx->LFSR_S7 = pCtx->LFSR_S8;
    pCtx->LFSR_S8 = pCtx->LFSR_S9;
    pCtx->LFSR_S9 = pCtx->LFSR_S10;
    pCtx->LFSR_S10 = pCtx->LFSR_S11;
    pCtx->LFSR_S11 = pCtx->LFSR_S12;
    pCtx->LFSR_S12 = pCtx->LFSR_S13;
    pCtx->LFSR_S13 = pCtx->LFSR_S14;
    pCtx->LFSR_S14 = pCtx->LFSR_S15;
    pCtx->LFSR_S15 = v;
} 

/* Clocking FSM.
 * Produces a 32-bit word F.
 * Updates FSM registers R1, R2, R3.
 * See Section 3.4.6.
 */
uint32 ClockFSM(tCryptoSnow3gCtx *pCtx)
{
    uint32 F = ( ( pCtx->LFSR_S15 + pCtx->FSM_R1 ) & 0xffffffff ) ^ pCtx->FSM_R2 ;
    uint32 r = ( pCtx->FSM_R2 + ( pCtx->FSM_R3 ^ pCtx->LFSR_S5 ) ) & 0xffffffff ;

    pCtx->FSM_R3 = S2(pCtx->FSM_R2);
    pCtx->FSM_R2 = S1(pCtx->FSM_R1);
    pCtx->FSM_R1 = r;

    return F;
}

/* Initialization.
 * Input k[4]: Four 32-bit words making up 128-bit key.
 * Input IV[4]: Four 32-bit words making 128-bit initialization variable.
 * Output: All the LFSRs and FSM are initialized for key generation.
 * See Section 4.1.
 */
void Initialize(tCryptoSnow3gCtx *pCtx, uint32 k[4], uint32 IV[4])
{
    uint32 F = 0x0;
    uint8  i = 0;

    pCtx->LFSR_S15 = k[3] ^ IV[0];
    pCtx->LFSR_S14 = k[2];
    pCtx->LFSR_S13 = k[1];
    pCtx->LFSR_S12 = k[0] ^ IV[1];

    pCtx->LFSR_S11 = k[3] ^ 0xffffffff;
    pCtx->LFSR_S10 = k[2] ^ 0xffffffff ^ IV[2];
    pCtx->LFSR_S9  = k[1] ^ 0xffffffff ^ IV[3];
    pCtx->LFSR_S8  = k[0] ^ 0xffffffff;

    pCtx->LFSR_S7 = k[3];
    pCtx->LFSR_S6 = k[2];
    pCtx->LFSR_S5 = k[1];
    pCtx->LFSR_S4 = k[0];

    pCtx->LFSR_S3 = k[3] ^ 0xffffffff;
    pCtx->LFSR_S2 = k[2] ^ 0xffffffff;
    pCtx->LFSR_S1 = k[1] ^ 0xffffffff;
    pCtx->LFSR_S0 = k[0] ^ 0xffffffff;

    pCtx->FSM_R1 = 0x0;
    pCtx->FSM_R2 = 0x0;
    pCtx->FSM_R3 = 0x0;

    for(i=0;i<32;i++)
    {
        F = ClockFSM(pCtx);
        ClockLFSRInitializationMode(pCtx, F);
    }
}

/* Generation of Keystream.
 * input n: number of 32-bit words of keystream.
 * input z: space for the generated keystream, assumes 
 *          memory is allocated already.
 * output: generated keystream which is filled in z 
 * See section 4.2.
 */
void GenerateKeystream(tCryptoSnow3gCtx *pCtx, uint32 n, uint32 *ks)
{
    uint32 t = 0;
    uint32 F = 0x0;

    ClockFSM(pCtx); /* Clock FSM once. Discard the output. */
    ClockLFSRKeyStreamMode(pCtx); /* Clock LFSR in keystream mode once. */

    for ( t=0; t<n; t++)
    {
        F = ClockFSM(pCtx);             /* STEP 1 */
        ks[t] = F ^ pCtx->LFSR_S0;      /* STEP 2 */
        /* Note that ks[t] corresponds to z_{t+1} in section 4.2 */
        ClockLFSRKeyStreamMode(pCtx);   /* STEP 3 */
    }
}

/* Initialization.
 * Input k[4]: Four 32-bit words making up 128-bit key.
 * Input IV[4]: Four 32-bit words making 128-bit initialization variable.
 * Output: All the LFSRs and FSM are initialized for key generation.
 * See Section 4.1.
 */
void crypto_snow_3g_initialize(tCryptoSnow3gCtx *pCtx, uint32 k[4], uint32 IV[4])
{
    Initialize(pCtx, k, IV);
}

/* Generation of Keystream.
 * input n: number of 32-bit words of keystream.
 * input z: space for the generated keystream, assumes 
 *          memory is allocated already.
 * output: generated keystream which is filled in z 
 * See section 4.2.
 */
void crypto_snow_3g_generateKeystream(tCryptoSnow3gCtx *pCtx, uint32 n, uint32 *ks)
{
    GenerateKeystream(pCtx, n, ks);
}

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
)
{
    tCryptoSnow3gCtx  ctx;
    uint32 *KS = NULL;
    uint32  K[4];
    uint32  IV[4];
    uint32  n;
    uint32  i;


    /* Initialisation */
    n = ((size << 3) + 31 ) / 32;

    /* Load the confidentiality key for SNOW 3G initialization as in section 3.4. */
    for (i=0; i<4; i++)
    {
        K[3-i] = (pKey[4*i  ] << 24) ^
                 (pKey[4*i+1] << 16) ^
                 (pKey[4*i+2] <<  8) ^
                 (pKey[4*i+3]      );
    }

    /* Prepare the initialization vector (IV) for SNOW 3G initialization as in section 3.4. */
    IV[3] = count;
    IV[2] = (bearer << 27) | ((dir & 0x1) << 26);

    IV[1] = IV[3];
    IV[0] = IV[2];

    /* Run SNOW 3G algorithm to generate sequence of key stream bits KS*/
    crypto_snow_3g_initialize(&ctx, K, IV);

    KS = (uint32 *)malloc(4*n);
    crypto_snow_3g_generateKeystream(&ctx, n, (uint32 *)KS); 

    /* Exclusive-OR the input data with keystream to generate the output bit stream */
    for (i=0; i<n; i++)
    {
        pData[4*i  ] ^= (uint8)(KS[i] >> 24) & 0xff;
        pData[4*i+1] ^= (uint8)(KS[i] >> 16) & 0xff;
        pData[4*i+2] ^= (uint8)(KS[i] >>  8) & 0xff;
        pData[4*i+3] ^= (uint8)(KS[i]      ) & 0xff;
    }

    free( KS );
}

