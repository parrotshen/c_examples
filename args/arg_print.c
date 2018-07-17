#include <stdio.h>
#include <stdarg.h>


void my_printf(int sequence, char *format, ...)
{
   va_list args;

   printf("[log][%d] ", sequence);

   va_start(args, format);
   vprintf(format, args);
   va_end(args);
}

int main(void)
{
   static int i = 1;

   my_printf(i++, "Hello ... my printf\n");
   my_printf(i++, "%d, %d, %d\n", 10, 100, 1000);
   my_printf(i++, "Goodbye ... my printf\n");

   return 0;
}

