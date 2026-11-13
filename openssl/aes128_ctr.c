#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/evp.h>


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


unsigned char g_plain[48] = {
 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
 0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C,
 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
 0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11,
 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF
};
unsigned char g_cipher[48] = {
 0x87, 0x4D, 0x61, 0x91, 0xB6, 0x20, 0xE3, 0x26,
 0x1B, 0xEF, 0x68, 0x64, 0x99, 0x0D, 0xB6, 0xCE,
 0x98, 0x06, 0xF6, 0x6B, 0x79, 0x70, 0xFD, 0xFF,
 0x86, 0x17, 0x18, 0x7B, 0xB9, 0xFF, 0xFD, 0xFF,
 0x5A, 0xE4, 0xDF, 0x3E, 0xDB, 0xD5, 0xD3, 0x5E,
 0x5B, 0x4F, 0x09, 0x02, 0x0D, 0xB0, 0x3E, 0xAB
};
unsigned char g_K[16] = {
 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};
unsigned char g_IV[16] = {
 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};


int main(int argc, char *argv[])
{
    EVP_CIPHER_CTX *ctx;
    unsigned char in[48];
    unsigned char out[48];
    int outLen;
    int inLen;


    dump("K", g_K, 16);
    dump("IV", g_IV, 16);

    printf("AES-128 CTR encryption:\n");
    memset(in, 0, 48);
    memset(out, 0, 48);
    memcpy(in, g_plain, 48);
    inLen = 48;
    dump("IN", in, inLen);

    if ((ctx = EVP_CIPHER_CTX_new()) == NULL)
    {
        printf("ERR: EVP_CIPHER_CTX_new\n");
        return -1;
    }

    if ((EVP_EncryptInit_ex(
             ctx,
             EVP_aes_128_ctr(),
             NULL,
             g_K,
             g_IV)) != 1)
    {
        printf("ERR: EVP_EncryptInit_ex\n");
        EVP_CIPHER_CTX_free( ctx );
        return -1;
    }

    if ((EVP_EncryptUpdate(
             ctx,
             out,
             &outLen,
             in,
             inLen)) != 1)
    {
        printf("ERR: EVP_EncryptUpdate\n");
        EVP_CIPHER_CTX_free( ctx );
        return -1;
    }

    EVP_CIPHER_CTX_free( ctx );
    dump("OUT", out, outLen);


    printf("AES-128 CTR decryption:\n");
    memset(in, 0, 48);
    memset(out, 0, 48);
    memcpy(in, g_cipher, 48);
    inLen = 48;
    dump("IN", in, inLen);

    if ((ctx = EVP_CIPHER_CTX_new()) == NULL)
    {
        printf("ERR: EVP_CIPHER_CTX_new\n");
        return -1;
    }

    if ((EVP_DecryptInit_ex(
             ctx,
             EVP_aes_128_ctr(),
             NULL,
             g_K,
             g_IV)) != 1)
    {
        printf("ERR: EVP_DecryptInit_ex\n");
        EVP_CIPHER_CTX_free( ctx );
        return -1;
    }

    if ((EVP_DecryptUpdate(
             ctx,
             out,
             &outLen,
             in,
             inLen)) != 1)
    {
        printf("ERR: EVP_DecryptUpdate\n");
        EVP_CIPHER_CTX_free( ctx );
        return -1;
    }

    EVP_CIPHER_CTX_free( ctx );
    dump("OUT", out, outLen);

    return 0;
}

