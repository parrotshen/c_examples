#include <stdio.h>

void div1(int a, int b)
{
    int c;

    printf("a = %d (0x%x)\n", a, a);
    printf("b = %d (0x%x)\n", b, b);

    c = a / b;

    printf("c = %d / %d = %d (0x%x)\n", a, b, c, c);
    printf("\n");
}

void div2(unsigned int a, int b)
{
    int c;

    printf("a = %d (0x%x)\n", a, a);
    printf("b = %d (0x%x)\n", b, b);

    c = a / b;

    printf("c = %d / %d = %d (0x%x)\n", a, b, c, c);
    printf("\n");
}

void div3(int a, unsigned int b)
{
    int c;

    printf("a = %d (0x%x)\n", a, a);
    printf("b = %d (0x%x)\n", b, b);

    c = a / b;

    printf("c = %d / %d = %d (0x%x)\n", a, b, c, c);
    printf("\n");
}

void div4(unsigned int a, unsigned int b)
{
    int c;

    printf("a = %d (0x%x)\n", a, a);
    printf("b = %d (0x%x)\n", b, b);

    c = a / b;

    printf("c = %d / %d = %d (0x%x)\n", a, b, c, c);
    printf("\n");
}

int main(void)
{
    printf("---------------------------------\n");
    printf(" (int) / (int)\n");
    printf("---------------------------------\n");
    div1( 300,  6);
    div1( 300, -6);
    div1(-300,  6);
    div1(-300, -6);

    printf("---------------------------------\n");
    printf(" (unsigned int) / (int)\n");
    printf("---------------------------------\n");
    div2( 300,  6);
    div2( 300, -6);
    div2(-300,  6);
    div2(-300, -6);

    printf("---------------------------------\n");
    printf(" (int) / (unsigned int)\n");
    printf("---------------------------------\n");
    div3( 300,  6);
    div3( 300, -6);
    div3(-300,  6);
    div3(-300, -6);

    printf("---------------------------------\n");
    printf(" (unsigned int) / (unsigned int)\n");
    printf("---------------------------------\n");
    div4( 300,  6);
    div4( 300, -6);
    div4(-300,  6);
    div4(-300, -6);

    return 0;
}

