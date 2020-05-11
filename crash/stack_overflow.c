#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


size_t stack_top;

size_t stack_size(void)
{
    int y = 0;
    return (stack_top - (size_t)&y);
}

void stack_overflow(int n)
{
    /* each call will consume 4000000+ bytes stack memory */
    int array[1000000];

    printf("%d) stack size %d\n", n, stack_size());

    array[0] = n;

    stack_overflow(n + 1);
}

int main(int argc, char *argv[])
{
    int x = 0;

    /* find the address of stack top */
    stack_top = (size_t)&x;

    stack_overflow( 1 );

    return 0;
}

