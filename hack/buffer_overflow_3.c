#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define PT_SIZE sizeof(void *)
#define UDP_DATA_SIZE 4095


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

unsigned char g_string[UDP_DATA_SIZE+1];
void *g_buf = NULL;
int g_fd = -1;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void dump_stack(void *pEBP, int offset, int size)
{
    unsigned char *pMem = (pEBP - offset);

    printf("\n");
    printf("Stack memory:\n");
    for (; pMem>=(unsigned char *)(pEBP - size); pMem-=PT_SIZE)
    {
        if (4 == PT_SIZE)
        {
            printf(
                " %p :: %02x%02x%02x%02x",
                pMem,
                pMem[3],
                pMem[2],
                pMem[1],
                pMem[0]
            );
        }
        else
        {
            printf(
                " %p :: %02x%02x%02x%02x %02x%02x%02x%02x",
                pMem,
                pMem[7],
                pMem[6],
                pMem[5],
                pMem[4],
                pMem[3],
                pMem[2],
                pMem[1],
                pMem[0]
            );
        }
        if (pMem == (pEBP + PT_SIZE + PT_SIZE))
        {
            printf("  1st argument");
        }
        else if (pMem == (pEBP + PT_SIZE))
        {
            printf("  [1;31mReturn address[0m");
        }
        else if (pMem == pEBP)
        {
            printf("  Caller's EBP");
        }
        else if (pMem == (pEBP - PT_SIZE - PT_SIZE - PT_SIZE))
        {
            printf("  1st local variable");
        }
        printf("\n");
    }
    printf("\n");
}

void hello(const char *input)
{
    unsigned char buf[128];
    register void *EBP asm("ebp");
    register void *ESP asm("esp");

    dump_stack(EBP, -16, 64);

    if ( input )
    {
        /* Buffer overflow while input length > 128 */
        strcpy((char *)buf, input);
    }

    printf("%s()\n", __func__);
    printf("\"%s\"\n", buf);
    printf(" %p :: %08lx  input\n", &input, (unsigned long)input);
    printf(" %p :: %08lx  EBP\n", EBP, *((unsigned long *)EBP));
    printf(" %p :: %02x%02x%02x%02x  buf\n", &(buf[0]), buf[3], buf[2], buf[1], buf[0]);
    printf(" %p :: %02x%02x%02x%02x\n", &(buf[4]), buf[7], buf[6], buf[5], buf[4]);
    printf(" %p :: %02x%02x%02x%02x\n", &(buf[8]), buf[11], buf[10], buf[9], buf[8]);
    printf(" %p :: %02x%02x%02x%02x\n", &(buf[12]), buf[15], buf[14], buf[13], buf[12]);
    printf(" %p :: ...\n", &(buf[16]));
    printf(" %p :: %08lx  ESP\n", ESP, *((unsigned long *)ESP));
    printf("\n");

    g_buf = &(buf[0]);

    dump_stack(EBP, -16, 64);
}

void terminate(int arg)
{
    if (g_fd > 0)
    {
        close( g_fd );
        g_fd = -1;
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(struct sockaddr_in);
    int port = 58168;
    int retval;
    int i;

    register void *EBP asm("ebp");
    register void *ESP asm("esp");


    printf("    &argc = %p\n", &argc);
    printf("     argv = %p\n", argv);
    for (i=0; i<4; i++)
    {
        printf(" &argv[%d] = %p\n", i, &argv[i]);
        if (i < argc)
        {
            printf("  argv[%d] = %p\n", i, argv[i]);
        }
    }
    printf("   printf = %p\n", printf);
    printf("    hello = %p\n", hello);
    printf("     main = %p\n", main);
    printf("     _END = %p\n", &&_END);
    printf("      EBP = %p\n", EBP);
    printf("    &addr = %p\n", &addr);
    printf(" &addrLen = %p\n", &addrLen);
    printf("    &port = %p\n", &port);
    printf("  &retval = %p\n", &retval);
    printf("       &i = %p\n", &i);
    printf("      ESP = %p\n", ESP);
    printf("\n");

    if (argc > 1)
    {
        port = atoi( argv[1] );
    }

    signal(SIGINT,  terminate);
    signal(SIGKILL, terminate);
    signal(SIGTERM, terminate);

    if ((g_fd=socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror( "socket" );
        return -1;
    }

    i = 1;
    retval = setsockopt(
                 g_fd,
                 SOL_SOCKET,
                 SO_REUSEADDR,
                 &i,
                 sizeof( i )
             );
    if (retval != 0)
    {
        perror( "setsockopt" );
        //close( g_fd );
        //return -1;
    }

    bzero(&addr, addrLen);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons( port );
    addr.sin_addr.s_addr = htonl( INADDR_ANY );

    if (bind(g_fd, (struct sockaddr *)&addr, addrLen) < 0)
    {
        perror( "bind" );
        close( g_fd );
        return -1;
    }

    printf("UDP ready to receive on prot %u ...\n\n", port);

    while ( 1 )
    {
        /*
         * Let EIP change to ESP after hello() returns.
         */
        printf("Try to modify the return address of hello():\n");
        printf(
            " [1;33m%02lx %02lx %02lx %02lx[0m -> ESP\n",
            (((unsigned long)ESP >> 24) & 0xFF),
            (((unsigned long)ESP >> 16) & 0xFF),
            (((unsigned long)ESP >>  8) & 0xFF),
            (((unsigned long)ESP      ) & 0xFF)
        );
        if ( g_buf )
        {
            printf(
                " [1;33m%02lx %02lx %02lx %02lx[0m -> &(buf[0])\n",
                (((unsigned long)g_buf >> 24) & 0xFF),
                (((unsigned long)g_buf >> 16) & 0xFF),
                (((unsigned long)g_buf >>  8) & 0xFF),
                (((unsigned long)g_buf      ) & 0xFF)
            );
        }
        printf("\n");

        bzero(&addr, addrLen);
        retval = recvfrom(
                     g_fd,
                     g_string,
                     UDP_DATA_SIZE,
                     0,
                     (struct sockaddr *)&addr,
                     &addrLen
                 );
        if (retval > 0)
        {
            g_string[retval] = 0x00;

            hello( (char *)g_string );

        _END:
            printf("\n");
            printf(
                "Receiving %d bytes from %s:%d\n",
                retval,
                inet_ntoa( addr.sin_addr ),
                ntohs( addr.sin_port )
            );
            printf("\n");
        }
        else
        {
            terminate( 0 );
            break;
        }
    }

    return 0;
}

