#include <stdio.h>

void hello(const char *caller)
{
   printf("%s: %s --> hello\n", __FILE__, caller);
}

void world(const char *caller)
{
   printf("%s: %s --> world\n", __FILE__, caller);
}

void foo(void)
{
   printf("%s: foo()\n", __FILE__);
   hello(__func__);
   world(__func__);
   printf("\n");
}

