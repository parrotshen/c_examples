#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int test = 123;

    printf("\ntest at %p\n\n", &test);

    /*** glibc detected *** ./free_error_1: munmap_chunk(): invalid pointer: 0xbf826ccc ***/
    {
        // free a non-allocated memory
        free( &test );
    }

    return 0;
}

