#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h> /* isspace */
#include <sys/stat.h>
#include <openssl/x509.h>


unsigned char convert_str_to_mac_addr(
     char          *pStr,
     unsigned char *pMacAddr,
     char          *pSeparator
)
{
    unsigned char i;
    unsigned int  value;

    if (!pMacAddr || !pStr)
    {
        return -1;
    }

    for (i = 0; i < 6; i++)
    {
        #ifdef MODULE
            value = simple_strtol( pStr, NULL, 16 );
        #else
            value = strtol( pStr, NULL, 16 );
        #endif

        if(value > 255)
        {
            return -1;
        }

        pStr += 2;

        if (i < 5)
        {
            if ( pSeparator )
            {
                if (*pStr++ != pSeparator[0])
                {
                    return -1;
                }
            }
        }
        else
        {
            char c = *pStr++;
            if (c != '\0' && !isspace(c))
            {
                return -1;
            }
        }

        pMacAddr[i] = value;
    }

    return 0;
}


typedef struct
{
    BIO      *certficate_bio;
    X509     *x509_cert;
    EVP_PKEY *public_key;
    RSA      *rsa;
    unsigned int  public_key_size;
    unsigned char ss_mac_address[6];
} crypto_context_t;

void crypto_free_context(void *pObj)
{
     crypto_context_t *pCtx = (crypto_context_t *)pObj;

     if ( pCtx )
     {
          if ( pCtx->rsa )
          {
                RSA_free( pCtx->rsa );
          }

          if ( pCtx->public_key )
          {
                EVP_PKEY_free( pCtx->public_key );
          }

          if ( pCtx->x509_cert )
          {
                X509_free( pCtx->x509_cert );
          }

          if (pCtx->certficate_bio)
          {
                BIO_free( pCtx->certficate_bio );
          }

          free( pCtx );
     }
}

unsigned int crypto_get_public_key_size(void *pObj)
{
    if ( pObj )
    {
        return ((crypto_context_t *)(pObj))->public_key_size;
    }

    return 0;
}

char *crypto_get_public_key(void *pObj, unsigned int *public_key_size)
{
    crypto_context_t *pCtx = (crypto_context_t *)pObj;

    if (pCtx && pCtx->public_key)
    {
        *public_key_size = pCtx->public_key_size;

        return (char*)(pCtx->public_key->pkey.rsa->n->d);
    }

    return NULL;
}

void *crypto_new_context(unsigned char *pCert, unsigned int certificate_size)
{
     crypto_context_t *pCtx = (crypto_context_t *)malloc( sizeof(crypto_context_t) );
     char *subject = NULL;

     if (pCtx == NULL)
     {
          printf("CERT> malloc(@ line %d) ... fail\n", __LINE__);
          goto end_1;
     }

     pCtx->certficate_bio = BIO_new_mem_buf(pCert, certificate_size);
     if (pCtx->certficate_bio == NULL)
     {
          printf("CERT> BIO_new_mem_buf ... fail\n");
          goto end_2;
     }

     pCtx->x509_cert = d2i_X509_bio(pCtx->certficate_bio, NULL);
     if (pCtx->x509_cert == NULL)
     {
          printf("CERT> d2i_X509_bio ... fail\n");
          goto end_3;
     }

     pCtx->public_key = X509_get_pubkey(pCtx->x509_cert);
     if (pCtx->public_key == NULL)
     {
          printf("CERT> X509_get_pubkey ... fail\n");
          goto end_4;
     }

     pCtx->rsa = EVP_PKEY_get1_RSA(pCtx->public_key);
     if (pCtx->rsa == NULL)
     {
          printf("CERT> EVP_PKEY_get1_RSA ... fail\n");
          goto end_5;
     }

     pCtx->public_key_size = RSA_size(pCtx->rsa);
     subject = X509_NAME_oneline(X509_get_subject_name(pCtx->x509_cert),NULL,0);
     printf("subject:\n");
     printf("%s\n", subject);

     if ( subject )
     {
          const char *asn1_common_name = "/CN=";
          char *mac_address = strstr(subject, asn1_common_name);
          printf("mac_address:\n");
          printf("%s\n", mac_address);

          /* the first one is the Serial Number */
          if (mac_address)
          {
                mac_address += strlen(asn1_common_name);
                mac_address = strstr(mac_address, asn1_common_name);

                if (mac_address)
                {
                     mac_address += strlen(asn1_common_name);

                     if (0 == convert_str_to_mac_addr(
                                  mac_address,
                                  pCtx->ss_mac_address,
                                  ":"))
                     {
                          ;
                     }
                }
          }

          OPENSSL_free( subject );
     }

     return (void *)pCtx;

end_5:
     EVP_PKEY_free( pCtx->public_key );
end_4:
     X509_free( pCtx->x509_cert );
end_3:
     BIO_free( pCtx->certficate_bio );
end_2:
     free( pCtx );
end_1:

     return NULL;
}


typedef struct
{
    unsigned char *value;
    unsigned int   len;
} cert_context_t;

cert_context_t *certificate_new_context(char *pName)
{
    cert_context_t *pCtx = NULL;
    FILE *pFile = NULL;
    struct stat stat_buf;
    long fsize;

    pFile = fopen(pName, "r+");
    if (pFile == NULL)
    {
        printf("Exit: cannot open file (%s) !!\n", pName);
        return pCtx;
    }

    if (stat(pName, &stat_buf) != 0)
    {
        printf("Exit: cannot get file size (%s) !!\n", pName);
        fclose( pFile );
        return pCtx;
    }
    fsize = stat_buf.st_size;

    pCtx = (cert_context_t *)malloc( sizeof(cert_context_t) );
    pCtx->value = malloc( fsize );
    pCtx->len    = fsize;

    /* Read the file contents into memory */
    fread(pCtx->value, fsize, fsize, pFile);
    fclose( pFile );

    return pCtx;
}


int main(int argc, char *argv[])
{
    cert_context_t *pCert = NULL;
    unsigned char  *pByte;
    int i;


    if (argc != 2)
    {
        printf("Usage: %s <cert_file.der>\n", argv[0]);
        printf("\n");
        return 0;
    }


    //[1] read certificate file (*.der)
    pCert = certificate_new_context( argv[1] );
    if ( pCert )
    {
        //[2] 
        // use crypto to decrypt X-509
        void *pCrypto = crypto_new_context(pCert->value, pCert->len);
        // extract the public key */
        if ( pCrypto )
        {
            unsigned int caPubKeySize = 0;
            char *pCaPubKey = crypto_get_public_key((void *)pCrypto, &caPubKeySize);
            if ( pCaPubKey )
            {
                printf("RSA> public key size = %d\n", caPubKeySize);
                printf("RSA> PK:\n");
                pByte = (unsigned char *)pCaPubKey;
                for (i=0; i<caPubKeySize; i++)
                {
                    if ((i != 0) && ((i % 16) == 0))
                    {
                        printf("\n");
                    }
                    printf(" %02X", *(pByte+i));
                }
                printf("\n");
                printf("RSA>\n");
            }
            else
            {
                printf("crypto_get_public_key() Error\n" );
                return -1;
            }
                                
            //free context
            crypto_free_context( (void *)pCrypto );
        }
        else
        {
            printf("crypto_new_context() Error\n" );
        }

        if (pCert->value != NULL)
        {
            free( pCert->value );
        }
        free( pCert );
    }

    return 0;
}

