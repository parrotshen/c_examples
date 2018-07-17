#include <stdio.h>
#include <stdarg.h>

void fun(int num_args, ...) /* the number of arguments is variable */
{
    va_list ap;
    int val;
    int i;

    va_start(ap, num_args);

    printf("Total has %d arguments:\n", num_args);
    for (i=0; i<num_args; i++)
    {
        val = va_arg(ap, int);
        printf("arg #%d -> %d\n", (i+1), val);
    }
    printf("\n");

    va_end(ap);
}

int main(void)
{
    int a = 123;
    int b = 456;
    int c = 789;

    fun(4, a, b, c, 0);

    return 0;
}

