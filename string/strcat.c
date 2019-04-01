#include <stdio.h>
#include <string.h>


char g_first[256] = "Hello ";
char g_second[]   = "world !!";


/*
 * char *strcat(
 *           char *dest,
 *           const char *src
 *       );
 */

int main(void)
{
    char *result;

    printf("[1] original string:\n");
    printf("%s\n", g_first);
    printf("%s\n\n", g_second);

    result = strcat(g_first, g_second);

    printf("[2] after strcat():\n");
    printf("%s\n\n", g_first);

    printf("[3] the return pointer:\n");
    printf("%s\n\n", result);

    return 0;
}

