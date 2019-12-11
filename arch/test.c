#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sched.h>
#include <errno.h>


#define BOOL_FALSE  0
#define BOOL_TRUE   1

typedef unsigned char   bool;
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef unsigned long   intptr;

typedef struct _tTest
{
    uint8   data1;  /* 1-byte */
    uint32  data2;  /* 4-byte */
    uint8   data3;  /* 1-byte */
} tTest;


/*
*  4-byte alignment:
*   10 00 00 00
*   20 21 22 23
*   30 00 00 00
*
*  not 4-byte alignment:
*   10
*   20 21 22 23
*   30
*/
uint8  _dummy;
uint8  _buf[16];


int test_bitsize(void)
{
    void *pointer = NULL;
    int   size;

    size = sizeof( pointer );
    printf("sizeof( void*     ) is %d\n", size);
    printf("sizeof( char      ) is %d\n", (int)sizeof(char));
    printf("sizeof( short     ) is %d\n", (int)sizeof(short));
    printf("sizeof( int       ) is %d\n", (int)sizeof(int));
    printf("sizeof( long      ) is %d\n", (int)sizeof(long));
    printf("sizeof( long long ) is %d\n", (int)sizeof(long long));
    printf("sizeof( size_t    ) is %d\n", (int)sizeof(size_t));
    printf("\n");

    switch ( size )
    {
        case 2:
            printf("This is a 16-bit machine !\n");
            break;

        case 4:
            printf("This is a 32-bit machine !\n");
            break;

        case 8:
            printf("This is a 64-bit machine !\n");
            break;

        default:
            printf("This is a unknown machine ! (size=%d)\n", size);
    }

    printf("\n");
    printf("\n");

    return size;
}

/*
*  unsigned int a = 0x12345678;
*
*  [Big Endian]
*
*  address high <- low:
*  +----+----+----+----+
*  | 78 | 56 | 34 | 12 |
*  +----+----+----+----+
*                  ^
*                pointer
*
*  [Little Endian]
*
*  address high <- low:
*  +----+----+----+----+
*  | 12 | 34 | 56 | 78 |
*  +----+----+----+----+
*                  ^
*                pointer
*/
void test_endian(void)
{
    uint32  var = 0x12345678; 
    uint8  *p = (uint8 *)&var;

    if (*p == 0x12)
    {
        printf("This is a Big Endian machine.\n\n");
    }
    else
    {
        printf("This is a Little Endian machine.\n\n");
    }

	printf("unsigned int var = 0x12345678;\n");
	printf("%p : 0x%02X\n", p,   *p);
	printf("%p : 0x%02X\n", p+1, *(p+1));
	printf("%p : 0x%02X\n", p+2, *(p+2));
	printf("%p : 0x%02X\n", p+3, *(p+3));

    printf("\n");
    printf("\n");
}

void test_alignment(void)
{
    tTest *pTest = NULL;
    uint8 *pBuf  = &(_buf[0]);

    bool   align4B;
    uint8  data1;
    uint32 data2;
    uint8  data3;
    int    size;
    int    i;


    _dummy = 0x5A;
    memset(pBuf, 0xFF, 16);

    /* [1] check data structure alignment */
    size = sizeof( tTest );
    if ((size % 4) == 0)
    {
        /* sizeof( tTest ) should be '12' */
        printf(
            "1. data structure ....... 4-byte alignment (size=%d)\n",
            size
        );
        align4B = BOOL_TRUE;
    }
    else
    {
        /* sizeof( tTest ) should be '6' */
        printf(
            "1. data structure ....... not 4-byte alignment (size=%d)\n",
            size
        );
        align4B = BOOL_FALSE;
    }

    /* [2] check variable address alignment */
    if ((((intptr)pBuf) % 4) == 0)
    {
        printf(
            "2. variable address ..... 4-byte alignment\n"
        );
    }
    else
    {
        printf(
            "2. variable address ..... not 4-byte alignment\n"
        );
    }

    /* [3] check 4-byte boundary alignment */
    if ( align4B )
    {
        while ((((intptr)pBuf) % 4) != 3)
        {
            /* move the start position to 0x.......3 */
            pBuf++;
        }
        /* .data1 */
        pBuf[0]  = 0x10;
        pBuf[1]  = 0x00;
        pBuf[2]  = 0x00;
        pBuf[3]  = 0x00;
        /* .data2 */
        pBuf[4]  = 0x20;
        pBuf[5]  = 0x21;
        pBuf[6]  = 0x22;
        pBuf[7]  = 0x23;
        /* .data3 */
        pBuf[8]  = 0x30;
        pBuf[9]  = 0x00;
        pBuf[10] = 0x00;
        pBuf[11] = 0x00;
    }
    else
    {
        while ((((intptr)pBuf) % 4) != 2)
        {
            /* move the start position to 0x.......2 */
            pBuf++;
        }
        /* .data1 */
        pBuf[0]  = 0x10;
        /* .data2 */
        pBuf[1]  = 0x20;
        pBuf[2]  = 0x21;
        pBuf[3]  = 0x22;
        pBuf[4]  = 0x23;
        /* .data3 */
        pBuf[5]  = 0x30;
    }

    pTest  = (tTest *)pBuf;
    data1  = pTest->data1;
    data2  = pTest->data2;
    data3  = pTest->data3;
    if ((0x10       == data1) &&
        (0x23222120 == data2) &&
        (0x30       == data3))
    {
        printf(
            "3. access boundary ...... not 4-byte limited\n"
        );
    }
    else
    {
        printf(
            "3. access boundary ...... 4-byte limited\n"
        );
        printf("data1 : 0x10       <--> %02X\n", data1);
        printf("data2 : 0x23222120 <--> %08X\n", data2);
        printf("data3 : 0x30       <--> %02X\n", data3);
    }
    printf("\n");


    /* dump buffer */
    pBuf = &(_buf[0]);
    while ((((intptr)pBuf) % 4) != 0)
    {
        pBuf--;
    }
    for (i=0; i<4; i++)
    {
        printf(
            "%p : %02X %02X %02X %02X\n",
            &(pBuf[i * 4]),
            pBuf[(i * 4)    ],
            pBuf[(i * 4) + 1],
            pBuf[(i * 4) + 2],
            pBuf[(i * 4) + 3]
        );
    }
    printf("\n\n");
}

void test_speed(void)
{
    struct timeval t1, t2;
    unsigned long long target;
    unsigned long long current;
    unsigned int interval = 1000; /* usec */
    unsigned int count;

    #if 0
    struct sched_param param;
    int prio;

    prio = sched_get_priority_max(SCHED_FIFO);
    if (prio < 0)
    {
        perror( "sched_get_priority_max" );
    }
    else
    {
        param.sched_priority = prio;
        if (sched_setscheduler(getpid(), SCHED_FIFO, &param) < 0)
        {
            perror( "sched_setscheduler" );
        }
    }
    #endif

    count = 0;
    gettimeofday(&t1, NULL);
    target = (((t1.tv_sec * 1000000LL) + t1.tv_usec) + interval);
    do
    {
        gettimeofday(&t2, NULL);
        current = ((t2.tv_sec * 1000000LL) + t2.tv_usec);
        count++;
    } while (current < target);

    printf("Run while-loop %u times during %u usec\n", count, interval);
    printf("\n");
    printf(
        "start time: %u.%u\n",
        (unsigned int)t1.tv_sec,
        (unsigned int)t1.tv_usec
    );
    printf(
        "end   time: %u.%u\n",
        (unsigned int)t2.tv_sec,
        (unsigned int)t2.tv_usec
    );
    printf("\n\n");
}

int main(int argc, char *argv[])
{
    printf("[30;47m[A] test the machine data size (bits) [0m\n\n");
    test_bitsize();

    printf("[30;47m[B] test the byte order               [0m\n\n");
    test_endian();

    printf("[30;47m[C] test the memory alignment         [0m\n\n");
    test_alignment();

    printf("[30;47m[D] test the CPU speed                [0m\n\n");
    test_speed();

    return 0;
}

