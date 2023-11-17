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


unsigned char g_plain[16] = {
 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A
};
unsigned char g_cipher[16] = {
 0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60,
 0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97
};
unsigned char g_K[16] = {
 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};


int main(int argc, char *argv[])
{
    AES_KEY encKey;
    AES_KEY decKey;
    unsigned char in[16];
    unsigned char out[16];


    AES_set_encrypt_key(g_K, 128, &encKey);
    AES_set_decrypt_key(g_K, 128, &decKey);
    dump("K", g_K, 16);


    printf("AES-128 ECB encryption:\n");
    memcpy(in, g_plain, 16);
    dump("IN", in, 16);
    memset(out, 0, 16);

    AES_ecb_encrypt(
        in,
        out,
        &encKey,
        AES_ENCRYPT
    );
    dump("OUT", out, 16);


    printf("AES-128 ECB decryption:\n");
    memcpy(in, g_cipher, 16);
    dump("IN", in, 16);
    memset(out, 0, 16);

    AES_ecb_encrypt(
        in,
        out,
        &decKey,
        AES_DECRYPT
    );
    dump("OUT", out, 16);


    return 0;
}

