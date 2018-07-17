#include <stdio.h>
#include <stdarg.h>


void test(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    while ( *fmt )
    {
        switch( *fmt++ )
        {
            case 's':  /* string */
                {
                    char *s = va_arg(ap, char *);
                    printf("string: %s\n", s);
                }
                break;

            case 'd':  /* int */
                {
                    int d = va_arg(ap, int);
                    printf("int: %d\n", d);
                }
                break;

            case 'c':  /* char */
                {
                    /* need a cast here since va_arg only
                       takes fully promoted types */
                    char c = (char)va_arg(ap, int);
                    printf("char: %c\n", c);
                }
                break;

            default:
                ;
        }
    }
    va_end(ap);

    printf("\n");
}


int main(void)
{
    int salary = 30000;
    int id = 15;
    int age = 20;
    int bonus = 5000;

    test("sd", "hello", id);
    test("dddd", salary, id, age, bonus);
    test("cc", 'x','.');

    return 0;
}

