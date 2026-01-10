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


#define WITH_AAD 1

#define KEY_SIZE sizeof(KEY)
unsigned char KEY[32] = {
 0xE3, 0xC0, 0x8A, 0x8F, 0x06, 0xC6, 0xE3, 0xAD,
 0x95, 0xA7, 0x05, 0x57, 0xB2, 0x3F, 0x75, 0x48,
 0x3C, 0xE3, 0x30, 0x21, 0xA9, 0xC7, 0x2B, 0x70,
 0x25, 0x66, 0x62, 0x04, 0xC6, 0x9C, 0x0B, 0x72
};

#define IV_SIZE sizeof(IV)
unsigned char IV[12] = {
 0x12, 0x15, 0x35, 0x24, 0xC0, 0x89, 0x5E, 0x81,
 0xB2, 0xC2, 0x84, 0x65
};

#if (WITH_AAD)
#define AAD_SIZE sizeof(AAD)
#else
#define AAD_SIZE 0
#endif
unsigned char AAD[28] = {
 0xD6, 0x09, 0xB1, 0xF0, 0x56, 0x63, 0x7A, 0x0D,
 0x46, 0xDF, 0x99, 0x8D, 0x88, 0xE5, 0x2E, 0x00,
 0xB2, 0xC2, 0x84, 0x65, 0x12, 0x15, 0x35, 0x24,
 0xC0, 0x89, 0x5E, 0x81
};

#define PT_SIZE sizeof(PT)
unsigned char PT[48] = {
 0x08, 0x00, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14,
 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24,
 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C,
 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34,
 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x00, 0x02
};

#define CT_SIZE sizeof(CT)
unsigned char CT[48] = {
 0xE2, 0x00, 0x6E, 0xB4, 0x2F, 0x52, 0x77, 0x02,
 0x2D, 0x9B, 0x19, 0x92, 0x5B, 0xC4, 0x19, 0xD7,
 0xA5, 0x92, 0x66, 0x6C, 0x92, 0x5F, 0xE2, 0xEF,
 0x71, 0x8E, 0xB4, 0xE3, 0x08, 0xEF, 0xEA, 0xA7,
 0xC5, 0x27, 0x3B, 0x39, 0x41, 0x18, 0x86, 0x0A,
 0x5B, 0xE2, 0xA9, 0x7F, 0x56, 0xAB, 0x78, 0x36
};

#define TAG_SIZE sizeof(TAG)
unsigned char TAG[16] = {
#if (WITH_AAD)
 0x5C, 0xA5, 0x97, 0xCD, 0xBB, 0x3E, 0xDB, 0x8D,
 0x1A, 0x11, 0x51, 0xEA, 0x0A, 0xF7, 0xB4, 0x36
#else
 0x21, 0xA5, 0x2A, 0x18, 0xAB, 0xFA, 0x85, 0x99,
 0x94, 0x5E, 0x82, 0xF4, 0x70, 0x05, 0x42, 0xB9
#endif
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
        .salg_type = "aead",
        .salg_name = "gcm(aes)"
    };
    struct af_alg_iv *iv;
    struct iovec iov;
    struct msghdr hdr;
    struct cmsghdr *chdr = NULL;
    unsigned char *cbuf;
    int cbufLen;
    unsigned char input[128];
    int inputLen;
    unsigned char output[128];
    int outputLen;
    int msgOpLen;
    int msgIvLen;
    int msgAssocLen;
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

    setsockopt(tfmfd, SOL_ALG, ALG_SET_KEY, KEY, KEY_SIZE);
    setsockopt(tfmfd, SOL_ALG, ALG_SET_AEAD_AUTHSIZE, NULL, TAG_SIZE);

    opfd = accept(tfmfd, NULL, 0);
    if (opfd < 0)
    {
        perror( "accept" );
        close( tfmfd );
        return -1;
    }

    msgOpLen = sizeof(unsigned int);
    msgIvLen = sizeof(struct af_alg_iv) + IV_SIZE;
    msgAssocLen = sizeof(unsigned int);
    cbufLen = CMSG_SPACE( msgOpLen ) + CMSG_SPACE( msgIvLen );
    if (AAD_SIZE > 0)
    {
        cbufLen += CMSG_SPACE( msgAssocLen );
    }
    cbuf = calloc(1, cbufLen);

    if (AAD_SIZE > 0)
    {
        memcpy(input, AAD, AAD_SIZE);
    }
    memcpy(input + AAD_SIZE, PT, PT_SIZE);
    inputLen = AAD_SIZE + PT_SIZE;
    iov.iov_base = input;
    iov.iov_len = inputLen;

    memset(&hdr, 0, sizeof(struct msghdr));
    hdr.msg_control = cbuf;
    hdr.msg_controllen = cbufLen;
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;

    chdr = CMSG_FIRSTHDR( &hdr );
    chdr->cmsg_level = SOL_ALG;
    chdr->cmsg_type = ALG_SET_OP;
    chdr->cmsg_len = CMSG_LEN( msgOpLen );
    *((unsigned int *)CMSG_DATA( chdr )) = ALG_OP_ENCRYPT;

    chdr = CMSG_NXTHDR(&hdr, chdr);
    chdr->cmsg_level = SOL_ALG;
    chdr->cmsg_type = ALG_SET_IV;
    chdr->cmsg_len = CMSG_LEN( msgIvLen );
    iv = (void *)CMSG_DATA( chdr );
    iv->ivlen = IV_SIZE;
    memcpy(iv->iv, IV, IV_SIZE);

    if (AAD_SIZE > 0)
    {
        chdr = CMSG_NXTHDR(&hdr, chdr);
        chdr->cmsg_level = SOL_ALG;
        chdr->cmsg_type = ALG_SET_AEAD_ASSOCLEN;
        chdr->cmsg_len = CMSG_LEN( msgAssocLen );
        *((unsigned int *)CMSG_DATA( chdr )) = AAD_SIZE;
    }

    if (sendmsg(opfd, &hdr, 0) < 0)
    {
        perror( "sendmsg" );
        close( opfd );
        close( tfmfd );
        free( cbuf );
        return -1;
    }

    outputLen = AAD_SIZE + CT_SIZE + TAG_SIZE;
    if (read(opfd, output, outputLen) < 0)
    {
        perror( "read" );
        close( opfd );
        close( tfmfd );
        free( cbuf );
        return -1;
    }

    close( opfd );
    close( tfmfd );
    free( cbuf );


    printf("CT output:\n");
    dump(output + AAD_SIZE, CT_SIZE);
    printf("TAG output:\n");
    dump(output + AAD_SIZE + CT_SIZE, TAG_SIZE);

    printf("CT answer:\n");
    dump(CT, CT_SIZE);
    printf("TAG answer:\n");
    dump(TAG, TAG_SIZE);

    return 0;
}
