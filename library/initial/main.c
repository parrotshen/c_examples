#include <stdio.h>
#include "initial.h"

int main(void)
{
   printf("%s: enter\n", __FILE__);

   stop();

   printf("%s: exit\n", __FILE__);
   return 0;
}
