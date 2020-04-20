#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void stack_overflow(int n, int size)
{
    int array[1000000];
    int seq;
    int acc;

    seq = (n + 1);
    acc = (size + sizeof(array));

    printf("%d) stack size %d\n", seq, acc);

    stack_overflow(seq, acc);
}

int main(int argc, char *argv[])
{
    stack_overflow(0, 0);

    return 0;
}

