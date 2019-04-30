#include <stdio.h>

#define SUM(a, b) \
  ({ \
      int sum = 0; \
      sum += a; \
      sum += b; \
      sum; \
  })

int main(void)
{
    int val;

    val = SUM(1000, 8);
    printf("SUM = %d\n", val);

    #undef  SUM(a, b)
    #define SUM(a, b) ((a) + (b))
    val = SUM(1000, 8);
    printf("SUM = %d\n", val);

    return 0;
}

