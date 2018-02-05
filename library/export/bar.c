#include <stdio.h>

void hello_world(void)
{
   printf("%s: Hello world !!\n\n", __FILE__);
}

void bar(void)
{
   printf("%s: bar()\n", __FILE__);
   hello_world();
}

