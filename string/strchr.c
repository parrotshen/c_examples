#include <stdio.h>
#include <string.h>

char symbol[] = { '@', '.', '9' };

int main(int argc, char *argv[])
{
    char *str = "123@example.com.tw";
    char *p;
    int i;

    for (i=0; i<3; i++)
    {
        printf("[%d] location of '%c'\n", (i + 1), symbol[i]);

        p = strchr(str, symbol[i]);

        printf(" p = %s\n", p);
        if ( p ) printf("*p = %c\n", *p);

        printf("\n");
    }

    if (argc > 1)
    {
        printf("[4] location of '%c'\n", argv[1][0]);

        p = strchr(str, argv[1][0]);

        printf(" p = %s\n", p);
        if ( p ) printf("*p = %c\n", *p);

        printf("\n");
    }

    return 0;
}
