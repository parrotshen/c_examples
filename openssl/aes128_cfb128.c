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
unsigned char g_cipher[52] = {
 0x31, 0x9C, 0x04, 0xA8, 0xFC, 0x0B, 0x55, 0xDE,
 0xB3, 0x63, 0x5C, 0x85, 0xF6, 0xC1, 0x83, 0x10,
 0x35, 0xE8, 0x91, 0x36, 0x22, 0x7D, 0x47, 0x25,
 0xC5, 0x33, 0x49, 0x6B, 0xB0, 0x07, 0xD1, 0x32,
 0x8C, 0x2F, 0x49, 0x93, 0xCC, 0xAD, 0xC4, 0xCF,
 0xBD, 0x29, 0x22, 0x71, 0x24, 0x87, 0x79, 0x66,
 0x88, 0x27, 0xC4, 0xB2
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
    unsigned char iv[16];
    unsigned char in[52];
    unsigned char out[52];
    int num;


    AES_set_encrypt_key(g_K, 128, &encKey);
    dump("K", g_K, 16);


    printf("AES-128 CFB128 encryption:\n");
    num = 0;
    memcpy(iv, g_IV, 16);
    memcpy(in, g_plain, 52);
    dump("IN", in, 52);
    memset(out, 0, 52);

    AES_cfb128_encrypt(
        in,
        out,
        52,
        &encKey,
        iv,
        &num,
        AES_ENCRYPT
    );
    dump("OUT", out, 52);


    printf("AES-128 CFB128 decryption:\n");
    num = 0;
    memcpy(iv, g_IV, 16);
    memcpy(in, g_cipher, 52);
    dump("IN", in, 52);
    memset(out, 0, 52);

    AES_cfb128_encrypt(
        in,
        out,
        52,
        &encKey,
        iv,
        &num,
        AES_DECRYPT
    );
    dump("OUT", out, 52);


    return 0;
}

