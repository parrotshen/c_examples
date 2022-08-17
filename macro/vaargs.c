#include <stdio.h>

/* ...: arbitrary number of arguments */
#define debug(...)  printf("[debug] " __VA_ARGS__)

int main(void)
{
    debug("One\n");
    debug("Two   %d\n", 1);
    debug("Three %d %d\n", 1, 2);
    debug("Four  %d %d %d\n", 1, 2, 3);

    return 0;
}

