#include <stdio.h>
#include <stdlib.h>


void dump(char *name, void *addr, unsigned int len)
{
   unsigned char *p = (unsigned char *)addr;
   unsigned int   i;

   printf("DUMP> %s\n", name);
   if (p == NULL)
   {
      printf("DUMP> NULL pointer\n");
      printf("\n");
   }

   for (i=0; i<len; i++)
   {
      if ((i != 0) && ((i % 16) == 0))
         printf("\n");
      printf(" %02X", p[i]);
   }
   printf("\n");
   printf(" (%u bytes)\n", len);
   printf("DUMP>\n");
   printf("\n");
}

int main(void)
{
   /*
   *  81985529216486895 / 64692944896 = 1267302
   */
   unsigned long long a = 0x0123456789abcdefLL;
   unsigned long long b = 0x0000000f10000000LL;
   unsigned long long c = 0LL;


   printf("sizeof(long long) = %d\n\n", sizeof(a));
   c = a / b;

   printf("a = %llu\n", a);
   dump("a", &a, sizeof(a));

   printf("b = %llu\n", b);
   dump("b", &b, sizeof(b));

   printf("c = %llu\n", c);
   dump("c", &c, sizeof(c));

   return(0);
}

