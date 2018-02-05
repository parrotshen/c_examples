#include <stdio.h>

void hello_world(void)
{
   printf("%s: Hello world !!\n\n", __FILE__);
}

void foo(void)
{
   printf("%s: foo()\n", __FILE__);
   hello_world();
}

