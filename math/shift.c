#include <inttypes.h>
#include <stdio.h>

int main(void)
{
    uint8_t val = 0xff;
    uint64_t a;
    uint64_t b;
    int i;

    printf("shift  a                 b\n");
    for (i=0; i<8; i++)
    {
        a = ((uint64_t)val << (i * 8));
        b = (          val << (i * 8));

        printf("%4d   %016llx  %016llx\n", (i * 8), a, b);
    }
    printf("\n");

    return 0;
}

