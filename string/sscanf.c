#include <stdio.h>
#include <string.h>


/*
 * int sscanf(
 *         const char *str,
 *         const char *format,
 *         ...
 *     );
 */

int main(void)
{
    char  protocol[32];
    char  site[256];
    char  path[256];


    /* %[*][width][modifiers]type */
    sscanf(
        "http://www.example.com/guest/test/hello.txt",
        "%[^:]:%*2[/]%[^/]/%[a-zA-Z0-9._/-]",
        protocol,
        site,
        path
    );
    printf("protocol = %s\n", protocol);
    printf("site     = %s\n", site);
    printf("path     = %s\n", path);
    printf("\n");

    return 0;
}

