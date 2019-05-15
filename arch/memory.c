#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

/*
* High address +===================+
*              |    Environment    | Command-line arguments
*              |                   | and environment variables
*              +===================+
*              |       Stack       |
*              |- - - - - - - - - -|
*              |         |         |
*              |         v         |
*              |                   |
*              |                   |
*              |         ^         |
*              |         |         |
*              |- - - - - - - - - -|
*              |        Heap       |
*              +===================+
*              |        BSS        | Un-initialized data
*              +===================+
*              |        Data       | Initialized data
*              +-------------------+
*              |        Text       |
*              |                   |
* Low address  +===================+
*/

#define PAGEMAP_LENGTH  (8)
#define PAGE_SHIFT      (12)

char string[] = "Hello world !!\n";

uintptr_t vtop(uintptr_t vaddr)
{
    uintptr_t paddr = 0;
    unsigned long long frame = 0;
    unsigned int offset = 0;
    FILE *pagemap;

    /* https://www.kernel.org/doc/Documentation/vm/pagemap.txt */
    pagemap = fopen("/proc/self/pagemap", "rb");
    if ( pagemap )
    {
        offset = ((unsigned int)vaddr / sysconf(_SC_PAGESIZE) * PAGEMAP_LENGTH);

        if (fseek(pagemap, (unsigned int)offset, SEEK_SET) == 0)
        {
            fread(&frame, PAGEMAP_LENGTH, 1, pagemap);

            /* check if page present */
            if (frame & (1ULL << 63))
            {
                frame &= ((1ULL << 54) - 1);
                #if 0
                printf("page frame number 0x%llx\n", frame);
                #endif

                paddr = ((frame << PAGE_SHIFT) + ((unsigned int)vaddr % sysconf(_SC_PAGESIZE)));
            }
        }

        fclose( pagemap );
    }

    return paddr;
}

void dump(const void *pAddr, unsigned int size, int verbose)
{
    unsigned char *pByte = (unsigned char *)pAddr;
    int num;
    int i;

    if ( verbose )
    {
        if (pAddr == NULL)
        {
            printf("NULL\n\n");
            return;
        }

        num = ((size + (4 - 1)) / 4);
        for (i=0; i<num; i++)
        {
            printf(
                "%p : %02x %02x %02x %02x\n",
                pByte,
                pByte[0],
                pByte[1],
                pByte[2],
                pByte[3]
            );
            pByte += 4;
        }
    }
    printf("\n");
}

/*
*  cat /proc/$PID/maps
*
*  address            perms  offset    dev    inode  pathname
*  08048000-08056000  r-xp   00000000  03:0c  64593  /usr/sbin/gpm
*/
int main(int argc, char *argv[])
{
    static void *pAlloc;
    uintptr_t addr1;
    uintptr_t addr2;
    uintptr_t addr3;
    uintptr_t addr4;
    uintptr_t addr5;
    uintptr_t addr6;
    uintptr_t addr7;
    int verbose = 0;
    int hold = 0;
    char command[80];


    if (argc > 1)
    {
        verbose = atoi( argv[1] );
        if (argc > 2)
        {
            hold = atoi( argv[2] );
        }
    }

    printf("[1] Text segment\n");
    {
        addr1 = vtop( (uintptr_t)main );
        addr2 = vtop( (uintptr_t)printf );

        printf("main()   -> %p (%p)\n", main, (void *)addr1);
        printf("printf() -> %p (%p)\n", printf, (void *)addr2);
        dump(main, 32, verbose);
    }


    printf("[2] Initialized data segment\n");
    {
        addr3 = vtop( (uintptr_t)string );

        printf("string   -> %p (%p)\n", string, (void *)addr3);
        dump(string, sizeof(string), verbose);
    }


    printf("[3] Un-initialized data segment\n");
    {
        addr4 = vtop( (uintptr_t)&pAlloc );

        printf("&pAlloc  -> %p (%p)\n", &pAlloc, (void *)addr4);
        dump(&pAlloc, 16, verbose);
    }


    printf("[4] Heap\n");
    pAlloc = malloc( 32 );
    if ( pAlloc )
    {
        int i;

        addr5 = vtop( (uintptr_t)pAlloc );

        for (i=0; i<32; i++)
        {
            ((unsigned char *)pAlloc)[i] = (i + 1);
        }

        printf("pAlloc   -> %p (%p)\n", pAlloc, (void *)addr5);
        dump(pAlloc, 32, verbose);

        free( pAlloc );
    }


    printf("[5] Stack\n");
    {
        addr6 = vtop( (uintptr_t)&addr1 );

        printf("&addr1   -> %p (%p)\n", &addr1, (void *)addr6);
        dump(&addr1, 16, verbose);
    }


    printf("[6] Environment\n");
    {
        addr7 = vtop( (uintptr_t)&argc );

        printf("&argc    -> %p (%p)\n", &argc, (void *)addr7);
        dump(&argc, 16, verbose);
    }


    printf("\n");
    printf("PID: %d\n", getpid());
    sprintf(command, "cat /proc/%d/maps", getpid());
    system( command );
    printf("\n");

    if ( hold )
    {
        printf("Press ENTER to continue ...\n");
        getchar();
    }

    return 0;
}

