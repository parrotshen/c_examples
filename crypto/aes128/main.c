#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto_aes.h"


uint8 _PTEXT[] = {
 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A
};

uint8 _CTEXT[] = {
 0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60,
 0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97
};

uint8 _KEY[CRYPTO_AES_128_KEY_SIZE] = {
 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

int main(void)
{
    tCryptoAesCtx  ctx;
    uint8  I[CRYPTO_AES_BLOCK_SIZE];
    uint8  O[CRYPTO_AES_BLOCK_SIZE];
    int    size;


    crypto_aes_init();

    if (0 != crypto_aes_setKey(&ctx, _KEY, CRYPTO_AES_128_KEY_SIZE))
    {
        printf("ERR: fail to set key (AES)\n");
        return -1;
    }

    size = sizeof( _PTEXT );


    /* AES ECB 128-bit encryption */
    memset(O, 0x00, CRYPTO_AES_BLOCK_SIZE);
    memset(I, 0x00, CRYPTO_AES_BLOCK_SIZE);
    memcpy(I, _PTEXT, size);

    crypto_aes_encrypt(&ctx, O, I);

    printf("[Encryption] ciphertext:\n");
    util_dump(O, size);


    /* AES ECB 128-bit decryption */
    memset(O, 0x00, CRYPTO_AES_BLOCK_SIZE);
    memset(I, 0x00, CRYPTO_AES_BLOCK_SIZE);
    memcpy(I, _CTEXT, size);

    crypto_aes_decrypt(&ctx, O, I);

    printf("[Decryption] plaintext:\n");
    util_dump(O, size);


    return 0;
}

