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
    *   a9993e36 4706816a ba3e2571 7850c26c 9cd0d89d
    */
    unsigned char digest[SHA_DIGEST_LENGTH];
    unsigned char message[64] = "abc";
    unsigned long length = 3;

#if 1
    // unsigned char *SHA1(
    //                    const unsigned char *d,
    //                    unsigned long        n,
    //                    unsigned char       *md
    //                );
    SHA1(message, length, digest);
#else
    SHA_CTX sha1;
    SHA1_Init( &sha1 );
    SHA1_Update(&sha1, message, length);
    SHA1_Final(digest, &sha1);
#endif
    mem_dump("SHA1 digest", digest, SHA_DIGEST_LENGTH);

    return 0;
}
