#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("\n");
    printf("ARG MAX .................... %ld\n", sysconf(_SC_ARG_MAX));
    printf("CHILD MAX .................. %ld\n", sysconf(_SC_CHILD_MAX));
    printf("HOST NAME MAX .............. %ld\n", sysconf(_SC_HOST_NAME_MAX));
    printf("LOGIN NAME MAX ............. %ld\n", sysconf(_SC_LOGIN_NAME_MAX));
    printf("NGROUPS MAX ................ %ld\n", sysconf(_SC_NGROUPS_MAX));
    printf("CLOCK TICKS ................ %ld\n", sysconf(_SC_CLK_TCK));
    printf("OPEN MAX ................... %ld\n", sysconf(_SC_OPEN_MAX));
    printf("PAGE SIZE .................. %ld\n", sysconf(_SC_PAGE_SIZE));
    printf("STREAM MAX ................. %ld\n", sysconf(_SC_STREAM_MAX));
    printf("SYMBOLIC LINKS MAX ......... %ld\n", sysconf(_SC_SYMLOOP_MAX));
    printf("TTY NAME MAX ............... %ld\n", sysconf(_SC_TTY_NAME_MAX));
    printf("POSIX VERSION .............. %ld\n", sysconf(_SC_VERSION));
    printf("POSIX.2 VERSION ............ %ld\n", sysconf(_SC_2_VERSION));
    printf("PROCESSORS CONFIG .......... %ld\n", sysconf(_SC_NPROCESSORS_CONF));
    printf("PROCESSORS ONLINE .......... %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
    printf("\n");

    return 0;
}

