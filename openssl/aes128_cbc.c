#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/aes.h>


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


char g_plain[52] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
unsigned char g_cipher[64] = {
 0x94, 0x09, 0x19, 0x32, 0x4E, 0x15, 0xBB, 0xB8,
 0x4C, 0x7C, 0xF7, 0x7D, 0xBC, 0x11, 0x0A, 0x7C,
 0xE2, 0xA4, 0xA3, 0x52, 0xE2, 0x9F, 0xBE, 0xFF,
 0x7D, 0x3D, 0xF1, 0x5E, 0x32, 0xB7, 0xD3, 0xE2,
 0x0D, 0x5F, 0x20, 0xD4, 0x08, 0xBB, 0x3A, 0x84,
 0xD6, 0x1B, 0xC7, 0x37, 0x9E, 0x3C, 0x78, 0x21,
 0xB1, 0x6D, 0x72, 0xC5, 0xBB, 0x12, 0xC1, 0x93,
 0xBE, 0xD8, 0x3E, 0xA3, 0x0D, 0xA8, 0xEE, 0x7A
};
unsigned char g_K[16] = {
 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};
unsigned char g_IV[16] = {
 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};


int main(int argc, char *argv[])
{
    AES_KEY encKey;
    AES_KEY decKey;
    unsigned char iv[16];
    unsigned char in[64];
    unsigned char out[64];


    AES_set_encrypt_key(g_K, 128, &encKey);
    AES_set_decrypt_key(g_K, 128, &decKey);
    dump("K", g_K, 16);


    printf("AES-128 CBC encryption:\n");
    memcpy(iv, g_IV, 16);
    memset(in, 0, 64);
    memcpy(in, g_plain, 52);
    dump("IN", in, 52);
    memset(out, 0, 64);

    AES_cbc_encrypt(
        in,
        out,
        64,
        &encKey,
        iv,
        AES_ENCRYPT
    );
    dump("OUT", out, 64);


    printf("AES-128 CBC decryption:\n");
    memcpy(iv, g_IV, 16);
    memcpy(in, g_cipher, 64);
    dump("IN", in, 64);
    memset(out, 0, 64);

    AES_cbc_encrypt(
        in,
        out,
        64,
        &decKey,
        iv,
        AES_DECRYPT
    );
    dump("OUT", out, 52);


    return 0;
}

