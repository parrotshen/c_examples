#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>


void mem_dump(void *pAddr, unsigned int size)
{
    unsigned char *pByte = pAddr;
    unsigned int   i;

    if ( pByte )
    {
        for (i=0; i<size; i++)
        {
            if ((i != 0) && ((i % 16) == 0))
            {
                printf("\n");
            }
            printf(" %02X", pByte[i]);
        }
        printf("\n");
        printf(" (%u bytes)\n", size);
        printf("\n");
    }
}

unsigned char hex2dec(char hex)
{
    unsigned char dec = 0;

    switch ( hex )
    {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            dec = (hex - '0');
            break;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            dec = ((hex - 'a') + 10);
            break;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            dec = ((hex - 'A') + 10);
            break;
        default:
            ;
    }

    return dec;
}

int parse_hex_string(char *pStr, unsigned char *pBuf, int bufLen)
{
    unsigned char hex;
    int len = 0;
    int i;

    for (i=0; i<strlen(pStr); i+=2)
    {
        if (len >= bufLen)
        {
            printf("ERR: un-enough buffer size (%d)\n", bufLen);
            break;
        }

        hex = (hex2dec( pStr[i] ) << 4);
        hex += hex2dec( pStr[i + 1] );

        pBuf[len++] = hex;
    }

    return len;
}

void help(void)
{
    printf("Usage: ii2_test [OPTION]...\n");
    printf("\n");
    printf("  -b BUS       I2C bus interface.\n");
    printf("  -a ADDRESS   I2C device address (HEX string).\n");
    printf("  -w DATA      Write operation (HEX string).\n");
    printf("  -r BYTES     Read operation.\n");
    printf("  -h           The help message.\n");
    printf("\n");
    printf("EX: ii2_test -b 10 -a 5B -w FC00C01020\n");
    printf("EX: ii2_test -b 10 -a 5B -w FA -r 1\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    int bus = -1;
    unsigned char addr = 0;
    static unsigned char send[256];
    static unsigned char recv[256];
    int sendLen = 0;
    int recvLen = 0;
    int wFlag = 0;
    int rFlag = 0;

    struct i2c_msg i2cMsg[2];
    struct i2c_rdwr_ioctl_data i2cRdWr;
    char devName[16];
    int fd;
    int ch;
    int rc;


    if (1 == argc)
    {
        help();
        return -1;
    }

    opterr = 0;
    while ((ch=getopt(argc, argv, "b:a:w:r:h")) != -1)
    {
        switch ( ch )
        {
            case 'b':
                bus = atoi( optarg );
                break;

            case 'a':
                rc = parse_hex_string(optarg, &addr, 1);
                if (rc != 1)
                {
                    printf("ERR: illegal ADDRESS\n\n");
                    return 0;
                }
                break;

            case 'w':
                wFlag = 1;
                sendLen = parse_hex_string(optarg, send, 256);
                if (0 == sendLen)
                {
                    printf("ERR: illegal write DATA\n\n");
                    return 0;
                }
                break;

            case 'r':
                rFlag = 1;
                recvLen = atoi( optarg );
                if ((recvLen <= 0) || (recvLen > 256))
                {
                    printf("ERR: illegal read BYTES\n\n");
                    return 0;
                }
                break;

            case 'h':
            default:
                help();
                return 0;
        }
    }

    if ((-1 == bus) ||
        ( 0 == addr) ||
        ((0 == wFlag) && (0 == rFlag)))
    {
        help();
        return 0;
    }


    #if 0  /* debug */
    printf("\n");
    printf("bus  :  %d\n",bus);
    printf("addr :  0x%x\n",addr);
    if ( wFlag )
    {
        printf("write:\n");
        mem_dump(send, sendLen);
    }
    if ( rFlag )
    {
        printf("read :  %d\n", recvLen);
    }
    printf("\n");
    #endif


    sprintf(devName, "/dev/i2c-%d", bus);
    fd = open(devName, O_RDWR);
    if (fd < 0)
    {
        printf("ERR: could not open %s\n\n", devName);
        return -1;
    }

    if (( wFlag ) && ( rFlag ))
    {
        i2cMsg[0].addr = addr;
        i2cMsg[0].flags = 0;
        i2cMsg[0].len = sendLen;
        i2cMsg[0].buf = send;
        i2cMsg[1].addr = addr;
        i2cMsg[1].flags = (I2C_M_RD | I2C_M_NOSTART);
        i2cMsg[1].len = recvLen;
        i2cMsg[1].buf = recv;

        i2cRdWr.nmsgs = 2;
        i2cRdWr.msgs = i2cMsg;

        rc = ioctl(fd, I2C_RDWR, (unsigned long)&i2cRdWr);
        if (rc < 0)
        {
            perror("ioctl (RDWR)");
            close( fd );
            return -1;
        }

        printf("Read data:\n");
        mem_dump(recv, recvLen);
    }
    else if ( wFlag )
    {
        i2cMsg[0].addr = addr;
        i2cMsg[0].flags = 0;
        i2cMsg[0].len = sendLen;
        i2cMsg[0].buf = send;

        i2cRdWr.nmsgs = 1;
        i2cRdWr.msgs = i2cMsg;

        rc = ioctl(fd, I2C_RDWR, (unsigned long)&i2cRdWr);
        if (rc < 0)
        {
            perror("ioctl (WR)");
            close( fd );
            return -1;
        }
    }
    else if ( rFlag )
    {
        i2cMsg[0].addr = addr;
        i2cMsg[0].flags = I2C_M_RD;
        i2cMsg[0].len = recvLen;
        i2cMsg[0].buf = recv;

        i2cRdWr.nmsgs = 1;
        i2cRdWr.msgs = i2cMsg;

        rc = ioctl(fd, I2C_RDWR, (unsigned long)&i2cRdWr);
        if (rc < 0)
        {
            perror("ioctl (RD)");
            close( fd );
            return -1;
        }

        printf("Read data:\n");
        mem_dump(recv, recvLen);
    }

    close( fd );

    return 0;
}

