#include <stdio.h>
#include <string.h>

char g_string[] = "strstr() - locate a substring";

void show_result(char *found, char *needle)
{
    int len;
    int i;

    printf("\"%s\"\n", g_string);
    if ( found )
    {
        len = (found - g_string);
        for (i=0; i<len; i++)
        {
            printf(" ");
        }
        printf(" ^\n");
        for (i=0; i<len; i++)
        {
            printf(" ");
        }
    }
    printf(" %p \"[1;35m%s[0m\"\n\n", found, needle);
}

/*
 * char *strstr(
 *           const char *haystack,
 *           const char *needle
 *       );
 */

int main(int argc, char *argv[])
{
    char *substring[3] = { "strstr", "substring", "hello" };
    char *found;
    int i;

    printf("\"%s\"\n", g_string);
    printf(" ^\n");
    printf(" %p [1;33mg_string[0m\n\n", g_string);

    if (argc > 1)
    {
        found = strstr(g_string, argv[1]);
        show_result(found, argv[1]);
    }
    else
    {
        for (i=0; i<3; i++)
        {
            found = strstr(g_string, substring[i]);
            show_result(found, substring[i]);
        }
    }

    return 0;
}

