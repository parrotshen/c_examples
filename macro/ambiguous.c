#include <stdio.h>

void func(void)
{
    printf("This is a subroutine !!\n");
}

int main(void)
{
    #define func() printf("This is an expanded macro !!\n")
    func();

    /* don't expand the macro */
    (func)();

    return 0;
}

