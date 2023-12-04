#include <stdio.h>

#define OPTION 1
#if (OPTION == 1)
# define FOO "foo"
# define BAR "bar"
#elif (OPTION == 2)
# define FOO "hello"
# define BAR "world"
#elif (OPTION == 3)
# define FOO "abc"
# define BAR "xyz"
#else
# error Incorrect option !!!
#endif

int main(void)
{
    printf("%s %s\n", FOO, BAR);
    return 0;
}

