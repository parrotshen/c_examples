#include "FooWrapper.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int val = 0;

    if (argc > 1) val = atoi( argv[1] );

    Foo *foo = Foo_new();

    Foo_set(foo, val);
    printf("%d\n", Foo_get( foo ));

    Foo_delete( foo );

    return 0;
}

