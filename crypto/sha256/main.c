#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto_sha.h"


uint8 _MESSAGE[] = {
 'a', 'b', 'c'
};

uint8 _DIGEST[] = {
 0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA,
 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
 0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C,
 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD
};

int main(void)
{
    tCryptoSha256Ctx  ctx;
    uint8  buf[CRYPTO_SHA256_DIGEST_SIZE];
    int    size;


    memset(BUF, 0x00, CRYPTO_SHA256_DIGEST_SIZE);
    size = sizeof( _MESSAGE );

    /* SHA-256 hash function */
    crypto_sha256_init( &ctx );
    crypto_sha256_update(&ctx, _MESSAGE, size);
    crypto_sha256_final(&ctx, buf);

    printf("Message: \n");
    util_dump(_MESSAGE, size);

    printf("Digest:\n");
    util_dump(buf, CRYPTO_SHA256_DIGEST_SIZE);

    if ( memcmp(buf, _DIGEST, CRYPTO_SHA256_DIGEST_SIZE) )
    {
        printf("SHA-256 test ... ERROR\n");
        printf("Digest should be: \n");
        util_dump(_DIGEST, CRYPTO_SHA256_DIGEST_SIZE);
    }
    else
    {
        printf("SHA-256 test ... OK\n\n");
    }


    return 0;
}

