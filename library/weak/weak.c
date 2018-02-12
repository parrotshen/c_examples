#include <stdio.h>
#include "weak.h"

#if (__WEAK_SYMBOL__)
void hello_world(void)
{
    printf("\n");
    printf("%s: %s is a weak symbol\n", __FILE__, __func__);
    printf("\n");
}
#endif

