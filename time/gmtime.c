#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    struct tm *nPtr;
    time_t t;

    if (argc < 2)
    {
        printf("Usage: gmtime INTEGER_VALUE\n");
        printf("\n");
        return 0;
    }

    if ((strlen(argv[1]) > 2) &&
        (argv[1][0] == '0') && (argv[1][1] == 'x'))
    {
        sscanf(argv[1], "0x%lx", &t);
    }
    else
    {
        t = (time_t)atoi( argv[1] );
    }

    nPtr = gmtime( &t );

    printf("tm_sec  = %d\n", nPtr->tm_sec);          /* 0 ~ 59 */
    printf("tm_min  = %d\n", nPtr->tm_min);          /* 0 ~ 59 */
    printf("tm_hour = %d\n", nPtr->tm_hour);         /* 0 ~ 23 */
    printf("tm_mday = %d\n", nPtr->tm_mday);         /* 1 ~ 31 */
    printf("tm_mon  = %d\n", nPtr->tm_mon + 1);      /* 0 ~ 11 */
    printf("tm_year = %d\n", nPtr->tm_year + 1900);  /* year - 1900 */
    printf("tm_wday = %d\n", nPtr->tm_wday);         /* 0 ~ 6 */
    printf("tm_yday = %d\n", nPtr->tm_yday + 1);     /* 0 ~ 365 */
    
    return 0;
}

