#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/err.h>


int verifySignature(RSA *rsa, unsigned char *sig, int sigLen, char *text)
{
    EVP_MD_CTX *mdctx;
    EVP_PKEY *pkey;
    int error = 0;

    mdctx = EVP_MD_CTX_create();
    if (NULL == mdctx)
    {
        printf("ERR: EVP_MD_CTX_create\n");
        return -1;
    }

    pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, rsa);

    if (1 != EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, pkey)) 
    {
        EVP_MD_CTX_free(mdctx);
        printf("ERR: EVP_MD_CTX_create\n");
        return -1;
    }

    if (1 != EVP_DigestVerifyUpdate(mdctx, (void *)text, strlen(text)))
    {
        EVP_MD_CTX_free(mdctx);
        printf("ERR: EVP_DigestVerifyUpdate\n");
        return -1;
    }

    if (1 != EVP_DigestVerifyFinal(mdctx, sig, sigLen))
    {
        printf("ERR: EVP_DigestVerifyFinal\n");
        error = -1;
    }

    EVP_MD_CTX_destroy(mdctx);
    return error;
}

int main(int argc, char *argv[])
{
    char *pemFile = "rsa_public.pem";
    char *docFile = "document.txt";
    char *sigFile = "signature.txt";
    char text[2048];

    RSA *rsa = NULL;
    unsigned char sig[512] = {0};
    int sigLen = 0;


    if (argc > 3)
    {
        pemFile = argv[1];
        docFile = argv[2];
        sigFile = argv[3];
    }
    else if (argc > 2)
    {
        pemFile = argv[1];
        docFile = argv[2];
    }
    else if (argc > 1)
    {
        pemFile = argv[1];
    }


    /* [1] load public key */
    {
        FILE *fstream;
        BIO *tmp;

        fstream = fopen(pemFile, "r");
        if (NULL == fstream)
        {
            perror("fopen");
            return -1;
        }
        memset(text, 0, 2048);
        if (fread(text, 2047, 1, fstream) < 0)
        {
            perror("fread");
            fclose(fstream);
            return -1;
        }
        if (text[strlen(text)-1] == '\n') text[strlen(text)-1] = 0;
        fclose(fstream);

        tmp = BIO_new_mem_buf((void *)text, strlen(text));
        rsa = PEM_read_bio_RSAPublicKey(tmp, &rsa, NULL, NULL);
        if (NULL == rsa)
        {
            printf("ERR: PEM_read_bio_RSAPublicKey\n");
            BIO_free(tmp);
            return -1;
        }
        BIO_free(tmp);
    }


    /* [2] load signature */
    {
        FILE *fstream;
        BIO *mem;
        BIO *b64;
        BIO *tmp;

        fstream = fopen(sigFile, "r");
        if (NULL == fstream)
        {
            perror("fopen");
            RSA_free(rsa);
            return -1;
        }
        memset(text, 0, 2048);
        if (fread(text, 2047, 1, fstream) < 0)
        {
            perror("fread");
            fclose(fstream);
            RSA_free(rsa);
            return -1;
        }
        if (text[strlen(text)-1] == '\n') text[strlen(text)-1] = 0;
        //printf("%s\n\n", text);
        fclose(fstream);

        mem = BIO_new_mem_buf((void *)text, strlen(text));
        b64 = BIO_new(BIO_f_base64());
        tmp = BIO_push(b64, mem);

        BIO_set_flags(tmp, BIO_FLAGS_BASE64_NO_NL);
        sigLen = BIO_read(tmp, (void *)sig, strlen(text));
        sig[sigLen] = 0;
        BIO_free_all(tmp);
    }


    /* [3] load document and verify signature */
    {
        FILE *fstream;

        fstream = fopen(docFile, "r");
        if (NULL == fstream)
        {
            perror("fopen");
            RSA_free(rsa);
            return -1;
        }
        memset(text, 0, 2048);
        if (fread(text, 2047, 1, fstream) < 0)
        {
            perror("fread");
            fclose(fstream);
            RSA_free(rsa);
            return -1;
        }
        if (text[strlen(text)-1] == '\n') text[strlen(text)-1] = 0;
        //printf("%s\n\n", text);
        fclose(fstream);

        if (0 != verifySignature(rsa, sig, sigLen, text))
        {
            printf("Verify signature failure !\n\n");
        }
        else
        {
            printf("Verify signature success !\n\n");
        }
    }

    RSA_free(rsa);
    return 0;
}
