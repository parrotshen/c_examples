#include <stdio.h>
#include <openssl/sha.h>


void mem_dump(char *name, void *addr, int size)
{
    unsigned char *byte = addr;
    int i;

    if ( addr )
    {
        printf("%s (%d bytes)\n", name, size);
        for (i=0; i<size; i++)
        {
            if ((i != 0) && ((i % 16) == 0)) printf("\n");
            printf(" %02X", byte[i]);
        }
        printf("\n\n");
    }
}

int main(void)
{
    /*
    * The digest should be:
    *   ba7816bf 8f01cfea 414140de 5dae2223
    *   b00361a3 96177a9c b410ff61 f20015ad
    */ 
    unsigned char digest[SHA256_DIGEST_LENGTH];
    unsigned char message[64] = "abc";
    unsigned long length = 3;

#if 1
    // unsigned char *SHA256(
    //                    const unsigned char *d,
    //                    size_t               n,
    //                    unsigned char       *md
    //                );
    SHA256(message, length, digest);
#else
    SHA256_CTX sha256;
    SHA256_Init( &sha256 );
    SHA256_Update(&sha256, message, length);
    SHA256_Final(digest, &sha256);
#endif
    mem_dump("SHA256 digest", digest, SHA256_DIGEST_LENGTH);

    return 0;
}
