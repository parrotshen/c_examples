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
 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

#define PT_SIZE sizeof(PT)
unsigned char PT[48] = {
 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
 0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C,
 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
 0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11,
 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF
};

#define CT_SIZE sizeof(CT)
unsigned char CT[48] = {
 0x87, 0x4D, 0x61, 0x91, 0xB6, 0x20, 0xE3, 0x26,
 0x1B, 0xEF, 0x68, 0x64, 0x99, 0x0D, 0xB6, 0xCE,
 0x98, 0x06, 0xF6, 0x6B, 0x79, 0x70, 0xFD, 0xFF,
 0x86, 0x17, 0x18, 0x7B, 0xB9, 0xFF, 0xFD, 0xFF,
 0x5A, 0xE4, 0xDF, 0x3E, 0xDB, 0xD5, 0xD3, 0x5E,
 0x5B, 0x4F, 0x09, 0x02, 0x0D, 0xB0, 0x3E, 0xAB
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
        .salg_name = "ctr(aes)"
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
