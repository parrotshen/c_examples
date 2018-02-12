#include <stdio.h>

void hello(const char *caller)
{
   printf("%s: %s ==> hello\n", __FILE__, caller);
}

void world(const char *caller)
{
   printf("%s: %s ==> world\n", __FILE__, caller);
}

void bar(void)
{
   printf("%s: bar()\n", __FILE__);
   hello(__func__);
   world(__func__);
   printf("\n");
}

