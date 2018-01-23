#include <stdio.h>

void test(void)
{
   printf("test ...\n");
}

/**
*  Library constructor function.
*/
void __attribute__ ((constructor)) my_init(void)
{
   printf("init ...\n");
}

/**
*  Library destructor function.
*/
void __attribute__ ((destructor)) my_uninit(void)
{
   printf("uninit ...\n");
}

