#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int num2digit(unsigned long long num, unsigned char *pBuf, int bufSize)
{
    /* unsigned 64-bit: 0 - 18446744073709551615 */
    unsigned char digit[32];
    int count = 0;
    int i;
    int j;

    digit[count++] = (num % 10);

    while ((num = (num / 10)) > 0)
    {
        //printf("%llu\n", num);
        digit[count++] = (num % 10);
    }

    if (count > bufSize)
    {
        printf(
            "ERR: un-enough buffer size (%d < %d)\n",
            bufSize,
            count
        );
    }

    for (i=0, j=(count-1); i<count && i<bufSize; i++, j--)
    {
        pBuf[i] = digit[j];
    }

    return count;
}

int main(int argc, char *argv[])
{
    /*
    * digit[0] = (val64 / 1000000000);
    * digit[1] = (val64 / 100000000) % 10;
    * digit[2] = (val64 / 10000000) % 10;
    * digit[3] = (val64 / 1000000) % 10;
    * digit[4] = (val64 / 100000) % 10;
    * digit[5] = (val64 / 10000) % 10;
    * digit[6] = (val64 / 1000) % 10;
    * digit[7] = (val64 / 100) % 10;
    * digit[8] = (val64 / 10) % 10;
    * digit[9] = (val64 % 10);
    */
    unsigned long long val64 = 1234567890LL;
    unsigned char digit[20];
    int len;
    int i;

    if (argc > 1)
    {
        val64 = atoll( argv[1] );
    }

    printf("%llu\n", val64);

    len = num2digit(val64, digit, 20);

    printf("\n");
    for (i=0; i<len; i++)
    {
        printf("%u\n", digit[i]);
    }
    printf("\n");

    printf("(%d digits)\n", len);
    return 0;
}
