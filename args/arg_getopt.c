#include <stdio.h>
#include <unistd.h>  /* getopt */


void help(void)
{
    printf("Usage: arg_getopt [OPTION]...\n");
    printf("\n");
    printf("  -a VALUE_A   Value of option A.\n");
    printf("  -b VALUE_B   Value of option B.\n");
    printf("  -c VALUE_C   Value of option C.\n");
    printf("  -d VALUE_D   Value of option D.\n");
    printf("  -e           Present of option E.\n");
    printf("  -f           Present of option F.\n");
    printf("  -g           Present of option G.\n");
    printf("  -h           The help message.\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    int ch;


    if (1 == argc)
    {
        help();
        return -1;
    }

    opterr = 0;
    while ((ch=getopt(argc, argv, "a:b:c:d:efgh")) != -1)
    {
        switch ( ch )
        {
            case 'a':
                printf("a: \"%s\"\n", optarg);
                break;

            case 'b':
                printf("b: \"%s\"\n", optarg);
                break;

            case 'c':
                printf("c: \"%s\"\n", optarg);
                break;

            case 'd':
                printf("d: \"%s\"\n", optarg);
                break;

            case 'e':
                printf("e: present\n");
                break;

            case 'f':
                printf("f: present\n");
                break;

            case 'g':
                printf("g: present\n");
                break;

            case 'h':
                help();
                return 0;

            default:
                printf("%c: \"%s\"\n", ch, optarg);
        }
    }


    return 0;
}

