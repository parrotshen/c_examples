#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct test_t
{
    unsigned int  len;   // 4-byte
    unsigned char val[]; // 0-byte
};


int main(int argc, char *argv[])
{
    unsigned char buf[4+16];
    struct test_t *test;
    int i;

    test = (void *)buf;
    test->len = 16;
    memset(test->val, 0x88, test->len);
    test->val[0]  = 0xbe;
    test->val[1]  = 0xef;
    test->val[14] = 0xca;
    test->val[15] = 0xfe;

    printf("sizeof(struct test_t): %ld\n",sizeof(struct test_t));
    printf("buf[20]:\n");
    for (i=0; i<sizeof(buf); i++)
    {
        printf(" %02x", buf[i]);
    }
    printf("\n");

    return 0;
}
