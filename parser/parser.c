#include <stdio.h>
#include "utility.h"

void display_token(char *pStr, int len, int count)
{
    printf("%s\n", pStr);
}

void display_line(char *pStr, int len, int count)
{
    #if 0
    printf("%d  ", count);
    printf("%s\n", pStr);
    #else
    parse_string_into_token(pStr, display_token);
    printf("\n");
    #endif
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        printf("\n");
        return -1;
    }

    parse_file_into_line(argv[1], display_line);

    return 0;
}

