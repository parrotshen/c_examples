#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


unsigned char g_number[8];


static void handler(int sig)
{
    if (SIGBUS == sig)
    {
        printf("BUS error\n\n");
        exit( -1 );
    }
}

int main(int argc, char *argv[])
{
    int *p;


    #if defined(__GNUC__) 
    {
        #if defined(__i386__) 
        /* enable alignment checking on x86 */
        __asm__("pushf\norl $0x40000,(%esp)\npopf"); 
        #elif defined(__x86_64__)  
         /* enable alignment checking on x86_64 */
        __asm__("pushf\norl $0x40000,(%rsp)\npopf"); 
        #endif
    }
    #endif 


    signal(SIGBUS, handler);

    p = (int *)&(g_number[0]);
    printf("p -> %p ", p);
    *p = 123;
    printf("[ %d ]\n", *p);

    p = (int *)&(g_number[4]);
    printf("p -> %p ", p);
    *p = 456;
    printf("[ %d ]\n", *p);

    /* try to access a non-alignment address */
    p = (int *)&(g_number[1]);
    printf("p -> %p ", p);
    *p = 789;
    printf("[ %d ]\n", *p);

    return 0;
}

