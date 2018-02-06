#include <stdio.h>

#if 1
void hello_world(void)
{
    printf("\n");
    printf("%s: %s()\n", __FILE__, __func__);
    printf("\n");
}
#endif

