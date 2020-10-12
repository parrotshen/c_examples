#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define PT_SIZE sizeof(void *)

// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

/*
 * Let EIP change to here after hello() returns.
 */
char g_callESP[] = "\xFF\xD4"; /* call *%esp */


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
    unsigned char buf[8] = {
        0x44, 0x33, 0x22, 0x11,
        0x88, 0x77, 0x66, 0x55
    };
    register void *EBP asm("ebp");
    register void *ESP asm("esp");

    dump_stack(EBP, -16, 56);

    if ( input )
    {
        /* Buffer overflow while input length > 8 */
        strcpy((char *)buf, input);
    }

    printf("%s()\n", __func__);
    printf("\"%s\"\n", buf);
    printf(" %p :: %08lx  input\n", &input, (unsigned long)input);
    printf(" %p :: %08lx  EBP\n", EBP, *((unsigned long *)EBP));
    printf(" %p :: %02x%02x%02x%02x  buf\n", &(buf[0]), buf[3], buf[2], buf[1], buf[0]);
    printf(" %p :: %02x%02x%02x%02x\n", &(buf[4]), buf[7], buf[6], buf[5], buf[4]);
    printf(" %p :: %08lx  ESP\n", ESP, *((unsigned long *)ESP));
    printf("\n");

    dump_stack(EBP, -72, 32);
}

int main(int argc, char *argv[])
{
    char *pStr = NULL;
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
    printf("g_callESP = %p\n", g_callESP);
    printf("   printf = %p\n", printf);
    printf("    hello = %p\n", hello);
    printf("     main = %p\n", main);
    printf("     _END = %p\n", &&_END);
    printf("      EBP = %p\n", EBP);
    printf("    &pStr = %p\n", &pStr);
    printf("       &i = %p\n", &i);
    printf("      ESP = %p\n", ESP);
    printf("\n");

    if (argc > 1)
    {
        pStr = argv[1];
    }
    hello( pStr );

_END:
    printf("\n");
    printf("Try to load shellcode as below:\n");
    printf(
        "%s [1;33m`echo -n -e \"11111111111111111111\\x%02lx\\x%02lx\\x%02lx\\x%02lx...\"`[0m\n",
        argv[0],
        (((unsigned long)g_callESP      ) & 0xFF),
        (((unsigned long)g_callESP >>  8) & 0xFF),
        (((unsigned long)g_callESP >> 16) & 0xFF),
        (((unsigned long)g_callESP >> 24) & 0xFF)
    );
    printf("\n");

    return 0;
}

