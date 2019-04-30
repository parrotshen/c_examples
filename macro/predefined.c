#include <stdio.h>

int main(void)
{
    printf("__LINE__     : %d\n", __LINE__);
    printf("__FILE__     : %s\n", __FILE__);
    printf("__func__     : %s\n", __func__);
    printf("__DATE__     : %s\n", __DATE__);
    printf("__TIME__     : %s\n", __TIME__);
    printf("__STDC__     : %d\n", __STDC__);
    #ifdef __TIMESTAMP__
    printf("__TIMESTAMP_ : %s\n", __TIMESTAMP__);
    #endif
    #ifdef __VERSION__
    printf("__VERSION__  : %s\n", __VERSION__);
    #endif

    return 0;
}

