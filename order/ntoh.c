#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <arpa/inet.h>


uint64_t ntohll(uint64_t val)
{
    return (((uint64_t)ntohl( val )) << 32) + ntohl(val >> 32);
}

int main(void)
{
    uint8_t   bit64[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
    uint8_t   bit32[4] = { 0x11, 0x22, 0x33, 0x44 };
    uint8_t   bit16[2] = { 0x11, 0x22 };
    uint64_t *pVal64;
    uint32_t *pVal32;
    uint16_t *pVal16;


    pVal64 = (uint64_t *)bit64;
    pVal32 = (uint32_t *)bit32;
    pVal16 = (uint16_t *)bit16;

    printf("\n");
    printf("---------+-------------------------+-------------------------------------------\n");
    printf(" Network | Memory                  | Value\n");
    printf("---------+-------------------------+-------------------------------------------\n");
    printf(
        "  64-bit | %02x %02x %02x %02x %02x %02x %02x %02x | %llu (0x%llx)\n",
        bit64[0], bit64[1], bit64[2], bit64[3],
        bit64[4], bit64[5], bit64[6], bit64[7],
        *pVal64,
        *pVal64
    );
    printf(
        "  32-bit | %02x %02x %02x %02x             | %u (0x%x)\n",
        bit32[0], bit32[1], bit32[2], bit32[3],
        *pVal32,
        *pVal32
    );
    printf(
        "  16-bit | %02x %02x                   | %u (0x%x)\n",
        bit16[0], bit16[1],
        *pVal16,
        *pVal16
    );
    printf("---------+-------------------------+-------------------------------------------\n");
    printf("\n");


    *pVal64 = ntohll( *pVal64 );
    *pVal32 = ntohl( *pVal32 );
    *pVal16 = ntohs( *pVal16 );

    printf("---------+-------------------------+-------------------------------------------\n");
    printf("    Host | Memory                  | Value\n");
    printf("---------+-------------------------+-------------------------------------------\n");
    printf(
        "  64-bit | %02x %02x %02x %02x %02x %02x %02x %02x | %llu (0x%llx)\n",
        bit64[0], bit64[1], bit64[2], bit64[3],
        bit64[4], bit64[5], bit64[6], bit64[7],
        *pVal64,
        *pVal64
    );
    printf(
        "  32-bit | %02x %02x %02x %02x             | %u (0x%x)\n",
        bit32[0], bit32[1], bit32[2], bit32[3],
        *pVal32,
        *pVal32
    );
    printf(
        "  16-bit | %02x %02x                   | %u (0x%x)\n",
        bit16[0], bit16[1],
        *pVal16,
        *pVal16
    );
    printf("---------+-------------------------+-------------------------------------------\n");
    printf("\n");


    return 0;
}

