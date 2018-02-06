#include <stdio.h>
#include "weak.h"

#if (__STRONG_SYMBOL__)
void hello_world(void)
{
    printf("\n");
    printf("%s: %s()\n", __FILE__, __func__);
    printf("\n");
}
#endif

int main(void)
{
    if ( hello_world ) hello_world();

    return 0;
}

