#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

int stop(void)
{
    struct termios org_opts, new_opts;
    int res = 0;
    int ch = 0;

    printf("Press any key to continue ...\n");

    /* store old settings */
    res = tcgetattr(STDIN_FILENO, &org_opts);

    /* set new terminal parms */
    memcpy(&new_opts, &org_opts, sizeof( new_opts ));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    ch = getchar();

    /* restore old settings */
    res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);

    return ch;
}

/**
*  Library constructor function.
*/
void __attribute__ ((constructor)) my_init(void)
{
    printf("\n");
    printf("%s: %s\n", __FILE__, __func__);
    printf("\n");
}

/**
*  Library destructor function.
*/
void __attribute__ ((destructor)) my_uninit(void)
{
    printf("\n");
    printf("%s: %s\n", __FILE__, __func__);
    printf("\n");
}

