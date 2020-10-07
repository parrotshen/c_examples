#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>


/*
* High address +===================+
*              |  Input arguments  |
*              |                   |
*              +-------------------+
*              |  Return address   |
*              +-------------------+ <- stack bottom \
*              |  Caller's EBP     |                 |
*              +-------------------+                 |
*              |  Local variables  |                 | Stack frame
*              |                   |                 |
*              |                   |                 |
*              +-------------------+ <- Stack top    /
*              |                   |
*              |                   |
*             ...                 ...
*              |                   |
*              |                   |
*  Low address +===================+
*/


#define PT_SIZE sizeof(void *)

void *g_EBP = NULL;
void *g_ESP = NULL;

void dump_stack(void *pBottom, void *pTop, char *pTitle, int label)
{
# define OFFSET_1ST_ARGUMENT        (PT_SIZE + PT_SIZE)
# define OFFSET_RETURN_ADDRESS      (PT_SIZE)
# define OFFSET_STACK_BASE          (0)
#if 0
# define OFFSET_1ST_LOCAL_VARIABLE -(PT_SIZE)
#else
# define OFFSET_1ST_LOCAL_VARIABLE -(PT_SIZE + PT_SIZE + PT_SIZE)
#endif

    unsigned char *pMem = (pBottom + (PT_SIZE * 4));
    unsigned char *pEBP = pBottom;
    unsigned char *pESP = pTop;

    printf("\n");
    printf("%s\n", pTitle);
    for (; pMem>=(unsigned char *)pTop; pMem-=PT_SIZE)
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
        if ( label )
        {
            if (pMem == (pEBP + OFFSET_1ST_ARGUMENT))
            {
                printf("  1st argument");
            }
            else if (pMem == (pEBP + OFFSET_RETURN_ADDRESS))
            {
                printf("  Return address");
            }
            else if (pMem == (pEBP + OFFSET_STACK_BASE))
            {
                printf("  [1;33mEBP[0m (Caller's EBP)");
            }
            else if (pMem == (pEBP + OFFSET_1ST_LOCAL_VARIABLE))
            {
                printf("  1st local variable");
            }
            else if (pMem == pESP)
            {
                printf("  [1;33mESP[0m");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void func2(unsigned int A, unsigned int B)
{
    register unsigned long EBP asm("ebp");
    register unsigned long ESP asm("esp");
    unsigned int C;
    unsigned int D;

    C = (A << 8);
    D = (B << 8);

    dump_stack((void *)EBP, (void *)ESP, (char *)__func__, 1);

    #if 0
    dump_stack(
        (g_EBP - (PT_SIZE * 2)),
        &D,
        "argc in main() to D in func2()",
        0
    );
    #endif
}

void func1(unsigned int A, unsigned int B)
{
    register unsigned long EBP asm("ebp");
    register unsigned long ESP asm("esp");
    unsigned int C;
    unsigned int D;

    C = (A << 8);
    D = (B << 8);

    dump_stack((void *)EBP, (void *)ESP, (char *)__func__, 1);

    func2(C, D);
}

int main(int argc, char *argv[])
{
    register unsigned long EBP asm("ebp");
    register unsigned long ESP asm("esp");
    unsigned int Z = 0xdeadbeef;

    g_EBP = (void *)EBP;
    g_ESP = (void *)ESP;

    if (argc > 1)
    {
        Z = atoi( argv[1] );
    }

    printf("EBP in main() = %p\n", g_EBP);
    printf("ESP in main() = %p\n", g_ESP);

    dump_stack((void *)EBP, (void *)ESP, (char *)__func__, 1);

    func1((Z >> 16), (Z & 0xFFFF));

    return 0;
}

