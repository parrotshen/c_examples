#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/core_names.h>
#include <openssl/evp.h>
#include <openssl/err.h>


static void dump(char *pName, void *pAddr, int size)
{
    unsigned char *p = pAddr;
    int i;

    printf("%s (%d bytes)\n", pName, size);
    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i%16) == 0)) printf("\n");
        printf("%02X ", p[i]);
    }
    printf("\n\n");
}


/* 60-byte Packet Encryption Using GCM-AES256 */
unsigned char g_K[32] = {
 0xE3, 0xC0, 0x8A, 0x8F, 0x06, 0xC6, 0xE3, 0xAD,
 0x95, 0xA7, 0x05, 0x57, 0xB2, 0x3F, 0x75, 0x48,
 0x3C, 0xE3, 0x30, 0x21, 0xA9, 0xC7, 0x2B, 0x70,
 0x25, 0x66, 0x62, 0x04, 0xC6, 0x9C, 0x0B, 0x72
};
unsigned char g_IV[12] = {
 0x12, 0x15, 0x35, 0x24, 0xC0, 0x89, 0x5E, 0x81,
 0xB2, 0xC2, 0x84, 0x65
};
unsigned char g_PT[48] = {
 0x08, 0x00, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14,
 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24,
 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C,
 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34,
 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x00, 0x02
};
unsigned char g_AAD[28] = {
 0xD6, 0x09, 0xB1, 0xF0, 0x56, 0x63, 0x7A, 0x0D,
 0x46, 0xDF, 0x99, 0x8D, 0x88, 0xE5, 0x2E, 0x00,
 0xB2, 0xC2, 0x84, 0x65, 0x12, 0x15, 0x35, 0x24,
 0xC0, 0x89, 0x5E, 0x81
};
unsigned char g_CT[48] = {
 0xE2, 0x00, 0x6E, 0xB4, 0x2F, 0x52, 0x77, 0x02,
 0x2D, 0x9B, 0x19, 0x92, 0x5B, 0xC4, 0x19, 0xD7,
 0xA5, 0x92, 0x66, 0x6C, 0x92, 0x5F, 0xE2, 0xEF,
 0x71, 0x8E, 0xB4, 0xE3, 0x08, 0xEF, 0xEA, 0xA7,
 0xC5, 0x27, 0x3B, 0x39, 0x41, 0x18, 0x86, 0x0A,
 0x5B, 0xE2, 0xA9, 0x7F, 0x56, 0xAB, 0x78, 0x36
};
unsigned char g_TAG[16] = {
 0x5C, 0xA5, 0x97, 0xCD, 0xBB, 0x3E, 0xDB, 0x8D,
 0x1A, 0x11, 0x51, 0xEA, 0x0A, 0xF7, 0xB4, 0x36
};

void encrypt(void)
{
    EVP_CIPHER_CTX *ctx;
    EVP_CIPHER *cipher;
    OSSL_PARAM params[2] = {
     OSSL_PARAM_END, OSSL_PARAM_END
    };
    unsigned char out[64];
    unsigned char tag[16];
    size_t ivLen = sizeof(g_IV);
    int outLen = 0;
    int tmpLen = 0;


    ctx = EVP_CIPHER_CTX_new();
    if (NULL == ctx)
    {
        printf("ERR: EVP_CIPHER_CTX_new\n");
        return;
    }

    cipher = EVP_CIPHER_fetch(NULL, "AES-256-GCM", NULL);
    if (NULL == cipher)
    {
        printf("ERR: EVP_CIPHER_fetch\n");
        EVP_CIPHER_CTX_free( ctx );
        return;
    }

    params[0]
     = OSSL_PARAM_construct_size_t(OSSL_CIPHER_PARAM_AEAD_IVLEN, &ivLen);

    if ( !EVP_EncryptInit_ex2(ctx, cipher, g_K, g_IV, params) )
    {
        printf("ERR: EVP_EncryptInit_ex2\n");
        EVP_CIPHER_free( cipher );
        EVP_CIPHER_CTX_free( ctx );
        return;
    }
    if ( !EVP_EncryptUpdate(ctx, NULL, &outLen, g_AAD, sizeof(g_AAD)) )
    {
        printf("ERR: EVP_EncryptUpdate\n");
        EVP_CIPHER_free( cipher );
        EVP_CIPHER_CTX_free( ctx );
        return;
    }
    if ( !EVP_EncryptUpdate(ctx, out, &outLen, g_PT, sizeof(g_PT)) )
    {
        printf("ERR: EVP_EncryptUpdate\n");
        EVP_CIPHER_free( cipher );
        EVP_CIPHER_CTX_free( ctx );
        return;
    }

    printf("AES-256 GCM encryption:\n");
    dump("OUT", out, outLen);

    if ( !EVP_EncryptFinal_ex(ctx, out, &tmpLen) )
    {
        printf("ERR: EVP_EncryptFinal_ex\n");
        EVP_CIPHER_free( cipher );
        EVP_CIPHER_CTX_free( ctx );
        return;
    }

    params[0]
     = OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG, tag, sizeof(tag));

    if ( !EVP_CIPHER_CTX_get_params(ctx, params) )
    {
        printf("ERR: EVP_CIPHER_CTX_get_params\n");
        EVP_CIPHER_free( cipher );
        EVP_CIPHER_CTX_free( ctx );
        return;
    }

    dump("TAG", tag, sizeof(tag));

    EVP_CIPHER_free( cipher );
    EVP_CIPHER_CTX_free( ctx );
}

void decrypt(void)
{
    EVP_CIPHER_CTX *ctx;
    EVP_CIPHER *cipher;
    OSSL_PARAM params[2] = {
     OSSL_PARAM_END, OSSL_PARAM_END
    };
    unsigned char out[64];
    size_t ivLen = sizeof(g_IV);
    int outLen = 0;


    ctx = EVP_CIPHER_CTX_new();
    if (NULL == ctx)
    {
        printf("ERR: EVP_CIPHER_CTX_new\n");
        return;
    }

    cipher = EVP_CIPHER_fetch(NULL, "AES-256-GCM", NULL);
    if (NULL == cipher)
    {
        printf("ERR: EVP_CIPHER_fetch\n");
        EVP_CIPHER_CTX_free( ctx );
        return;
    }

    params[0]
     = OSSL_PARAM_construct_size_t(OSSL_CIPHER_PARAM_AEAD_IVLEN, &ivLen);

    if ( !EVP_DecryptInit_ex2(ctx, cipher, g_K, g_IV, params) )
    {
        printf("ERR: EVP_DecryptInit_ex2\n");
        EVP_CIPHER_free( cipher );
        EVP_CIPHER_CTX_free( ctx );
        return;
    }
    if ( !EVP_DecryptUpdate(ctx, NULL, &outLen, g_AAD, sizeof(g_AAD)) )
    {
        printf("ERR: EVP_DecryptUpdate\n");
        EVP_CIPHER_free( cipher );
        EVP_CIPHER_CTX_free( ctx );
        return;
    }
    if ( !EVP_DecryptUpdate(ctx, out, &outLen, g_CT, sizeof(g_CT)) )
    {
        printf("ERR: EVP_DecryptUpdate\n");
        EVP_CIPHER_free( cipher );
        EVP_CIPHER_CTX_free( ctx );
        return;
    }

    printf("AES-256 GCM decryption:\n");
    dump("OUT", out, outLen);

    params[0]
     = OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG, g_TAG, sizeof(g_TAG));

    if ( !EVP_CIPHER_CTX_set_params(ctx, params) )
    {
        printf("ERR: EVP_CIPHER_CTX_set_params\n");
        EVP_CIPHER_free( cipher );
        EVP_CIPHER_CTX_free( ctx );
        return;
    }

    printf(
        "Tag verify: %s\n",
         EVP_DecryptFinal_ex(ctx, out, &outLen) > 0 ? "success" : "fail"
    );

    EVP_CIPHER_free( cipher );
    EVP_CIPHER_CTX_free( ctx );
}

int main(int argc, char *argv[])
{
    dump("K", g_K, sizeof(g_K));
    encrypt();
    decrypt();
    return 0;
}

