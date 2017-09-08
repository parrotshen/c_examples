#include <stdio.h>
#include <time.h>

int main(void)
{
   time_t  t;

   time( &t );

   fprintf(stdout, "%s\n", ctime( &t ));

   return 0;
}
