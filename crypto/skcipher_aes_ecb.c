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

#define PT_SIZE sizeof(PT)
unsigned char PT[16] = {
 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A
};

#define CT_SIZE sizeof(CT)
unsigned char CT[16] = {
 0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60,
 0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97
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
        .salg_name = "ecb(aes)"
    };
    struct iovec iov;
    struct msghdr hdr;
    struct cmsghdr *chdr;
    unsigned char cbuf[CMSG_SPACE( sizeof(unsigned int) )];
    unsigned char input[PT_SIZE];
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
