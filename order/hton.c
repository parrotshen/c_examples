#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <arpa/inet.h>


uint64_t htonll(uint64_t val)
{
    return (((uint64_t)htonl( val )) << 32) + htonl(val >> 32);
}
 
int main(void)
{
    uint64_t  val64 = 168LL;
    uint32_t  val32 = 168;
    uint16_t  val16 = 168;
    uint8_t  *pBit64;
    uint8_t  *pBit32;
    uint8_t  *pBit16;


    pBit64 = (uint8_t *)&val64;
    pBit32 = (uint8_t *)&val32;
    pBit16 = (uint8_t *)&val16;

    printf("\n");
    printf("---------+-------------------------+-------------------------------------------\n");
    printf("    Host | Memory                  | Value\n");
    printf("---------+-------------------------+-------------------------------------------\n");
    printf(
        "  64-bit | %02x %02x %02x %02x %02x %02x %02x %02x | %llu (0x%016llx)\n",
        pBit64[0], pBit64[1], pBit64[2], pBit64[3],
        pBit64[4], pBit64[5], pBit64[6], pBit64[7],
        val64,
        val64
    );
    printf(
        "  32-bit | %02x %02x %02x %02x             | %u (0x%08x)\n",
        pBit32[0], pBit32[1], pBit32[2], pBit32[3],
        val32,
        val32
    );
    printf(
        "  16-bit | %02x %02x                   | %u (0x%04x)\n",
        pBit16[0], pBit16[1],
        val16,
        val16
    );
    printf("---------+-------------------------+-------------------------------------------\n");
    printf("\n");


    val64 = htonll( val64 );
    val32 = htonl( val32 );
    val16 = htons( val16 );

    printf("---------+-------------------------+-------------------------------------------\n");
    printf(" Network | Memory                  | Value\n");
    printf("---------+-------------------------+-------------------------------------------\n");
    printf(
        "  64-bit | %02x %02x %02x %02x %02x %02x %02x %02x | %llu (0x%llx)\n",
        pBit64[0], pBit64[1], pBit64[2], pBit64[3],
        pBit64[4], pBit64[5], pBit64[6], pBit64[7],
        val64,
        val64
    );
    printf(
        "  32-bit | %02x %02x %02x %02x             | %u (0x%x)\n",
        pBit32[0], pBit32[1], pBit32[2], pBit32[3],
        val32,
        val32
    );
    printf(
        "  16-bit | %02x %02x                   | %u (0x%x)\n",
        pBit16[0], pBit16[1],
        val16,
        val16
    );
    printf("---------+-------------------------+-------------------------------------------\n");
    printf("\n");


    return 0;
}

