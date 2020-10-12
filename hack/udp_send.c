#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>  /* htons */
#include <arpa/inet.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define IS_UPPER_ALHPABET(ch)  ((ch >= 'A') && (ch <=  'Z'))
#define IS_LOWER_ALHPABET(ch)  ((ch >= 'a') && (ch <=  'z'))
#define IS_NUMBER(ch)          ((ch >= '0') && (ch <=  '9'))
#define IS_SPACE(ch)           ((ch == ' ') || (ch == '\t'))
#define LINE_SIZE     511
#define TOKEN_SIZE    31
#define UDP_BUF_SIZE  4095


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

unsigned char  g_buf[UDP_BUF_SIZE+1];
int  g_len = 0;

/* UDP socket */
int  g_sockUdp = -1;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void mem_dump(char *pLabel, void *pAddr, unsigned int size)
{
    unsigned char *pByte = (unsigned char *)pAddr;
    unsigned int   i;

    if (pByte == NULL)
    {
        printf("[ %s ]\n NULL\n\n", pLabel);
        return;
    }

    printf("[ %s ]\n", pLabel);
    for(i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }
        printf(" %02X", pByte[i]);
    }
    printf("\n (%u bytes)\n", size);
}

char *get_token(char *pLine, char *pToken, int tsize)
{
    char *pt = pToken;
    char *pl = pLine;
    int   i  = 0;

    if (0x0 == pLine[0])
    {
        /* This is a NULL line */
        pToken[0] = 0x0;
        return NULL;
    }

    /* Pass space and tab character */
    for (; *pl && IS_SPACE(*pl); pl++);

    /* Get the separation token */
    for (; *pl && !IS_SPACE(*pl) && i<tsize; pl++, i++)
    {
        *pt++ = *pl;
    }
    *pt = 0x0;

    return pl;
}

int read_line(FILE *pFile, char *pLine, int lsize)
{
    pLine[0] = 0x0;

    /* char *fgets(                                   */
    /*     char *s,      // character array to store  */
    /*     int   n,      // length to read            */
    /*     FILE *stream  // FILE pointer              */
    /* );                                             */
    fgets(pLine, lsize, pFile);
    if ( feof(pFile) )
    {
        return 0;
    }

    /* remove the CR/LF character */
    if ((strlen(pLine) > 0) && (pLine[strlen(pLine)-1] == 0x0a))
    {
        pLine[strlen(pLine)-1] = 0x0;
    }
    if ((strlen(pLine) > 0) && (pLine[strlen(pLine)-1] == 0x0d))
    {
        pLine[strlen(pLine)-1] = 0x0;
    }

    return 1;
}

int read_file(char *pName, unsigned char *pBuf, unsigned int bsize)
{
    unsigned char *byte = pBuf;
    unsigned int   count = 0;

    FILE *pFile = NULL;
    char  line[LINE_SIZE+1];
    char  token[TOKEN_SIZE+1];
    char *next;
    int   i;

    if ((pFile=fopen(pName, "r")) == NULL)
    {
        printf("ERR: cannot open file %s\n", pName);
        return 0;
    }

    /* start reading input file */
    while ( read_line(pFile, line, LINE_SIZE) )
    {
        next = line;

        do
        {
            next = get_token(next, token, TOKEN_SIZE);
            if ((token[0] == 0x0) || (token[0] == '#'))
            {
                /* ignore the comment and null line */
                break;
            }

            /* get the token and transfer to one byte */
            sscanf(token, "%x", &i);
            if ((count+1) > bsize)
            {
                printf("ERR: buffer size(%u) is un-enough\n", bsize);
                goto _EXIT_READ_FILE;
            }

            *byte++ = (i & 0xFF);
            count++;
        } while ( next );
    }

_EXIT_READ_FILE:
    fclose( pFile );
    return count;
}

int udp_sockSend(
             char  *pIp,
    unsigned short  port,
    unsigned char  *pData,
    unsigned short  size
)
{
    struct sockaddr_in  addr;
    socklen_t addrLen = sizeof(struct sockaddr_in);
    int  retval;


    if (NULL == pData)
    {
        printf("ERR: cannot send NULL data\n");
        return -1;
    }

    bzero(&addr, addrLen);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons( port );
    addr.sin_addr.s_addr = inet_addr( pIp );

    retval = sendto(
                 g_sockUdp,
                 pData,
                 size,
                 0,
                 (struct sockaddr *)&addr,
                 addrLen
             );
    if (retval < 0)
    {
        perror( "sendto" );
    }

    return retval;
}

int udp_sockInit(unsigned short port)
{
    //struct sockaddr_in  addr;
    //socklen_t addrLen = sizeof(struct sockaddr_in);

    g_sockUdp = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_sockUdp < 0)
    {
        perror( "socket" );
        return -1;
    }

    //bzero(&addr, addrLen);
    //addr.sin_family      = AF_INET;
    //addr.sin_port        = htons( port );
    //addr.sin_addr.s_addr = htonl( INADDR_ANY );

    //if (bind(g_sockUdp, (struct sockaddr *)&addr, addrLen) < 0)
    //{
    //    perror( "bind" );
    //    close( g_sockUdp );
    //    g_sockUdp = -1;
    //    return -1;
    //}

    return 0;
}

void udp_sockUninit(void)
{
    if (g_sockUdp > 0)
    {
        close( g_sockUdp );
        g_sockUdp = -1;
    }
}

int main(int argc,char *argv[])
{
    char *pText = "udp_send.txt";
    char *pIp = "127.0.0.1";
    int port = 58168;
    int retval;


    if (argc == 2)
    {
        pText = argv[1];
    }
    else if (argc == 3)
    {
        pText = argv[1];
        pIp = argv[2];
    }
    else if (argc == 4)
    {
        pText = argv[1];
        pIp = argv[2];
        port = atoi( argv[3] );
    }
    else
    {
        printf("Usage: udp_send TEXT_FILE\n");
        printf("     : udp_send TEXT_FILE IP_ADDRESS\n");
        printf("     : udp_send TEXT_FILE IP_ADDRESS PORT_NUMBER\n");
        printf("\n");
        return 0;
    }

    retval = udp_sockInit( 0 );
    if (retval != 0)
    {
        printf("ERR: fail to initial UDP socket (port %u)\n\n", port);
        udp_sockUninit();
        return -1;
    }

    if ((g_len=read_file(pText, g_buf, UDP_BUF_SIZE)) == 0)
    {
        printf("ERR: fail to open file %s\n\n", pText);
        udp_sockUninit();
        return -1;
    }

    retval = udp_sockSend(pIp, port, g_buf, g_len);
    if (retval < 0)
    {
        printf("ERR: fail to send data (error %d)\n\n", retval);
    }
    else
    {
        mem_dump(pText, g_buf, g_len);
    }

    udp_sockUninit();

    return 0;
}

