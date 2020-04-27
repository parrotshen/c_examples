#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int subroutine(int value)
{
    printf("%s: %d\n", __func__, value);

    return value;
}

int main(int argc, char *argv[])
{
    printf(
        "\n%d  %d  %d  %d  %d  %d  %d  %d\n\n",
        subroutine( 1 ),
        subroutine( 2 ),
        subroutine( 3 ),
        subroutine( 4 ),
        subroutine( 5 ),
        subroutine( 6 ),
        subroutine( 7 ),
        subroutine( 8 )
    );

    return 0;
}

