#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <linux/if_alg.h>


#ifndef AF_ALG
#define AF_ALG 38
#endif
#ifndef SOL_ALG 
#define SOL_ALG 279
#endif


#define KEY_SIZE sizeof(KEY)
unsigned char KEY[16] = {
 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

#define IV_SIZE sizeof(IV)
unsigned char IV[16] = {
 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

#define PT_SIZE sizeof(PT)
unsigned char PT[52] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

#define CT_SIZE sizeof(CT)
unsigned char CT[64] = {
 0x94, 0x09, 0x19, 0x32, 0x4E, 0x15, 0xBB, 0xB8,
 0x4C, 0x7C, 0xF7, 0x7D, 0xBC, 0x11, 0x0A, 0x7C,
 0xE2, 0xA4, 0xA3, 0x52, 0xE2, 0x9F, 0xBE, 0xFF,
 0x7D, 0x3D, 0xF1, 0x5E, 0x32, 0xB7, 0xD3, 0xE2,
 0x0D, 0x5F, 0x20, 0xD4, 0x08, 0xBB, 0x3A, 0x84,
 0xD6, 0x1B, 0xC7, 0x37, 0x9E, 0x3C, 0x78, 0x21,
 0xB1, 0x6D, 0x72, 0xC5, 0xBB, 0x12, 0xC1, 0x93,
 0xBE, 0xD8, 0x3E, 0xA3, 0x0D, 0xA8, 0xEE, 0x7A
};


void dump(void *ptr, int len)
{
    unsigned char *byte = ptr;
    int i;

    if ( byte )
    {
        for (i=0; i<len; i++)
        {
            if ((i != 0) && ((i % 8) == 0)) printf("\n");
            printf(" %02x", byte[i]);
        }
        printf("\n\n");
    }
}

int main(void)
{
    struct sockaddr_alg sa = {
        .salg_family = AF_ALG,
        .salg_type = "skcipher",
        .salg_name = "cbc(aes)"
    };
    struct af_alg_iv *iv;
    struct iovec iov;
    struct msghdr hdr;
    struct cmsghdr *chdr;
    unsigned char cbuf[CMSG_SPACE( sizeof(unsigned int) ) +
                       CMSG_SPACE( sizeof(struct af_alg_iv) + IV_SIZE )];
    unsigned char input[CT_SIZE];
    unsigned char output[CT_SIZE];
    int tfmfd;
    int opfd;


    tfmfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
    if (tfmfd < 0)
    {
        perror( "socket" );
        return -1;
    }

    if (bind(tfmfd, (struct sockaddr *)&sa, sizeof(sa)) != 0)
    {
        perror( "bind" );
        close( tfmfd );
        return -1;
    }

    if (setsockopt(tfmfd, SOL_ALG, ALG_SET_KEY, KEY, KEY_SIZE) != 0)
    {
        perror( "setsockopt" );
        close( tfmfd );
        return -1;
    }

    opfd = accept(tfmfd, NULL, 0);
    if (opfd < 0)
    {
        perror( "accept" );
        close( tfmfd );
        return -1;
    }


    memset(input, 0, sizeof(input));
    memcpy(input, PT, PT_SIZE);
    iov.iov_base = input;
    iov.iov_len = sizeof(input);

    memset(&hdr, 0, sizeof(struct msghdr));
    hdr.msg_control = cbuf;
    hdr.msg_controllen = sizeof(cbuf);
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;

    chdr = CMSG_FIRSTHDR( &hdr );
    chdr->cmsg_level = SOL_ALG;
    chdr->cmsg_type = ALG_SET_OP;
    chdr->cmsg_len = CMSG_LEN( sizeof(unsigned int) );
    *((unsigned int *)CMSG_DATA( chdr )) = ALG_OP_ENCRYPT;
    
    chdr = CMSG_NXTHDR(&hdr, chdr);
    chdr->cmsg_level = SOL_ALG;
    chdr->cmsg_type = ALG_SET_IV;
    chdr->cmsg_len = CMSG_LEN( sizeof(struct af_alg_iv) + IV_SIZE );
    iv = (void *)CMSG_DATA( chdr );
    iv->ivlen = IV_SIZE;
    memcpy(iv->iv, IV, IV_SIZE);

    if (sendmsg(opfd, &hdr, 0) < 0)
    {
        perror( "sendmsg" );
        close( opfd );
        close( tfmfd );
        return -1;
    }

    if (read(opfd, output, sizeof(output)) < 0)
    {
        perror( "read" );
        close( opfd );
        close( tfmfd );
        return -1;
    }

    close( opfd );
    close( tfmfd );


    printf("CT output:\n");
    dump(output, CT_SIZE);

    printf("CT answer:\n");
    dump(CT, CT_SIZE);

    return 0;
}
