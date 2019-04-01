#include <stdio.h>
#include <string.h>


unsigned char g_string[] =
 "The strtok() function parses a string into a sequence of tokens.\n"
 "On the first call to strtok() the string to be parsed should be specified in str.\n"
 "In each subsequent call that should parse the same string, str should be NULL.\n";

char g_delmit[] = " \t\n";


/*
 * char *strtok(
 *           char       *str,
 *           const char *delim
 *       );
 */

int main(void)
{
    char *token;

    printf("[1] original string:\n");
    printf("%s\n\n", g_string);

    token = strtok((char *)g_string, g_delmit);

    printf("[2] after strtok():\n");
    printf("%s\n\n", g_string);

    printf("[3] extract tokens:\n");
    printf("\n");
    while ( token )
    {
        printf("\"%s\"\n", token);
        token = strtok(NULL, g_delmit);
    }
    printf("\n");
    printf("\n");

    return 0;
}

