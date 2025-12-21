#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // bytes to be encoded
    unsigned char d[32] = {
        0,0,0,1,1,0,1,0, // 1Ah
        1,1,0,0,1,1,1,1, // CFh
        1,1,1,1,1,1,0,0, // FCh
        0,0,0,1,1,1,0,1  // 1Dh
    };
    unsigned char poly1 = 0x4F; // 1111001b
    unsigned char poly2 = 0x6D; // 1011011b
    unsigned char R[14];
    unsigned char G[32][64];
    unsigned char hex[8];
    int res[64];
    int i, j;

    for (i=0, j=0; i<7; i++)
    {
        R[j++] = (poly1 >> i) & 0x1;
        R[j++] = (poly2 >> i) & 0x1;
    }

    memset(G[0], 0, 64);
    memcpy(&G[0][0], R, 14);
    for (j=0; j<(32-7); j++)
    {
        i = j+1;
        memset(G[i], 0, 64);
        memcpy(&G[i][i*2], R, 14);
    }
    for (j=12; j>=2; j-=2)
    {
        i++;
        memset(G[i], 0, 64);
        memcpy(&G[i][64-j], R, j);
    }
    printf("In :");
    for (j=0; j<64; j++)
    {
        res[j] = 0;
        for (i=0; i<32; i++)
        {
            res[j] += d[i]*G[i][j];
        }
        res[j] = 1 - (res[j] & 0x1);
        if ((j != 0) && ((j % 8) == 0))
        {
            printf("\n    ");
        }
        printf(" %d", res[j]);
    }
    printf("\n\n");
    printf("Out:");
    for (i=0, j=0; i<8; i++, j+=8)
    {
        hex[i] = (res[j  ] << 7) |
                 (res[j+1] << 6) |
                 (res[j+2] << 5) |
                 (res[j+3] << 4) |
                 (res[j+4] << 3) |
                 (res[j+5] << 2) |
                 (res[j+6] << 1) |
                 (res[j+7]);
        printf(" %02x", hex[i]);
    }
    printf("\n");

    return 0;
}
