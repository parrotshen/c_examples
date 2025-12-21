#include <stdio.h>
#include <string.h>
#include <fec.h>

int main(int argc, char *argv[])
{
                   // V27POLYB  V27POLYA
    int polys[2] = {  0x4F,     0x6D     };
    unsigned char symbs[64+12];
    unsigned char in[8] = {
        0xFC, 0xA2, 0xB6, 0x3D, 0xB0, 0x0D, 0x97, 0x94
    };
    unsigned char out[4]; // 0x1ACFFC1D
    void *vp;
    int i, j, k = 0;

    #if 1
    memset(&symbs[64], 255, 12);
    #else
    memset(&symbs[64], 0, 12);
    #endif
    for (i=0; i<8; i++)
    {
        for (j=7; j>=0; j--)
        {
            symbs[k] = (in[i] >> j) & 0x1;
            #if 1
            symbs[k] = (1 - symbs[k]) * 255;
            #else
            symbs[k] = (symbs[k]) * 255;
            #endif
            k++;
        }
    }

    vp = create_viterbi27( 32 );
    if ( vp )
    {
        set_viterbi27_polynomial(polys);
        update_viterbi27_blk(vp, symbs, 32+6);
        chainback_viterbi27(vp, out, 32, 0);
        delete_viterbi27(vp);

        printf(
            "In : %02x %02x %02x %02x %02x %02x %02x %02x\n",
            in[0], in[1], in[2], in[3],
            in[4], in[5], in[6], in[7]
        );
        printf(
            "Out: %02x %02x %02x %02x\n",
            out[0], out[1], out[2], out[3]
        );
        printf("Ans: 1a cf fc 1d\n");
    }
    return 0;
}
