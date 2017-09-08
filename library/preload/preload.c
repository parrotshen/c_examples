#include <stdio.h>
#include <string.h>
#include <time.h>


/* Begin of preload time */
#define _MY_BIRTHDAY  124560000

time_t time (time_t *__timer)
{
    time_t t = 0;

    fprintf(stdout, "LD_PRELOAD> time\n");

    if ( __timer )
    {
        t = _MY_BIRTHDAY;
        *__timer = t;
    }

    return t;
}
/* End   of preload time */


/* Begin of preload localtime */
static struct tm  my_birthday = {
  .tm_sec   = 0,    /* Seconds. [0-60] (1 leap second) */
  .tm_min   = 0,    /* Minutes. [0-59] */
  .tm_hour  = 0,    /* Hours.   [0-23] */
  .tm_mday  = 13,   /* Day.     [1-31] */
  .tm_mon   = 11,   /* Month.   [0-11] */
  .tm_year  = 73,   /* Year - 1900.  */
  .tm_wday  = 4,    /* Day of week.  [0-6] */
  .tm_yday  = 347,  /* Days in year. [0-365] */
  .tm_isdst = 0,    /* DST.          [-1/0/1]   */
};

struct tm *localtime (__const time_t *__timer)
{
    fprintf(stdout, "LD_PRELOAD> localtime\n");

    if ( __timer )
        return &my_birthday;
    else
        return NULL;
}
/* End   of preload localtime */


/* Begin of preload printf */
int printf(const char *format, ...)
{
    fprintf(stdout, "LD_PRELOAD> printf\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "This is not printf() ...\n");
    fprintf(stdout, "\n");

    return 0;
}

int puts(const char *s)
{
    fprintf(stdout, "LD_PRELOAD> puts\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "This is not puts() ...\n");
    fprintf(stdout, "\n");

    return 0;
}
/* End   of preload printf */

