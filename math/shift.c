#include <inttypes.h>
#include <stdio.h>

int main(void)
{
    uint64_t a;
    uint64_t b;
    int i;

    printf("shift  a                 b\n");
    for (i=0; i<8; i++)
    {
        a = (UINT64_C(0xff) << (i * 8));
        b = (         0xff  << (i * 8));

        printf("%4d   %016llx  %016llx\n", (i * 8), a, b);
    }
    printf("\n");

    return 0;
}

