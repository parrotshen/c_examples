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


void b64Encode(unsigned char *data, int dataLen)
{
    char *str = NULL;
    int strLen;
    BIO *mem;
    BIO *b64;
    BIO *tmp;

    mem = BIO_new(BIO_s_mem());
    b64 = BIO_new(BIO_f_base64());
    tmp = BIO_push(b64, mem);
    BIO_set_flags(tmp, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(tmp, data, dataLen);
    BIO_flush(tmp);
    strLen = BIO_get_mem_data(mem, &str);
    str[strLen] = 0;

    printf("%s\n", str);
    printf("%d\n\n", strLen);

    BIO_free_all(tmp);
}

int signDocument(EVP_PKEY *pkey, char *text)
{
    EVP_MD_CTX *mdctx;
    unsigned char *sig = NULL;
    size_t sigLen = 0;

    printf("%s\n\n", text);

    mdctx = EVP_MD_CTX_create();
    if (mdctx == NULL)
    {
        printf("ERR: EVP_MD_CTX_create\n");
        return -1;
    }

    if (1 != EVP_DigestSignInit(mdctx, NULL, EVP_sha256(), NULL, pkey))
    {
        EVP_MD_CTX_destroy(mdctx);
        printf("ERR: EVP_DigestSignInit\n");
        return -1;
    }
    if (EVP_DigestSignUpdate(mdctx, (void *)text, strlen(text)) != 1)
    {
        EVP_MD_CTX_destroy(mdctx);
        printf("ERR: EVP_DigestSignUpdate\n");
        return -1;
    }
    if (EVP_DigestSignFinal(mdctx, NULL, &sigLen) != 1)
    {
        EVP_MD_CTX_destroy(mdctx);
        printf("ERR: EVP_DigestSignFinal\n");
        return -1;
    }
    if ( !(sig = (unsigned char *)OPENSSL_malloc(sigLen)) )
    {
        EVP_MD_CTX_destroy(mdctx);
        printf("ERR: OPENSSL_malloc\n");
        return -1;
    }
    if (1 != EVP_DigestSignFinal(mdctx, sig, &sigLen))
    {
        OPENSSL_free(sig);
        EVP_MD_CTX_destroy(mdctx);
        printf("ERR: EVP_DigestSignFinal\n");
        return -1;
    }

    b64Encode(sig, sigLen);

    OPENSSL_free(sig);
    EVP_MD_CTX_destroy(mdctx);
    return 0;
}

int main(int argc, char *argv[])
{
    char *pemFile = "rsa_private.pem";
    char *docFile = "document.txt";
    char text[2048];

    EVP_PKEY *pkey = NULL;


    if (argc > 2)
    {
        pemFile = argv[1];
        docFile = argv[2];
    }
    else if (argc > 1)
    {
        pemFile = argv[1];
    }


    /* [1] load private key */
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
        pkey = PEM_read_bio_PrivateKey(tmp, &pkey, NULL, NULL);
        if (NULL == pkey)
        {
            BIO_free(tmp);
            printf("ERR: PEM_read_bio_PrivateKey\n");
            return -1;
        }
        BIO_free(tmp);
    }


    /* [2] sign document */
    {
        FILE *fstream;

        fstream = fopen(docFile, "r");
        if (NULL == fstream)
        {
            perror("fopen");
            EVP_PKEY_free(pkey);
            return -1;
        }
        memset(text, 0, 2048);
        if (fread(text, 2047, 1, fstream) < 0)
        {
            perror("fread");
            fclose(fstream);
            EVP_PKEY_free(pkey);
            return -1;
        }
        if (text[strlen(text)-1] == '\n') text[strlen(text)-1] = 0;
        fclose(fstream);

        signDocument(pkey, text);
    }


    EVP_PKEY_free(pkey);
    return 0;
}
