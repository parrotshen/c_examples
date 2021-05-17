#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>


int main(void)
{
    char token[4] = { '-', '\\', '|', '/' };
    unsigned long count = 0;

    struct timeval tv;
    pid_t pid;
    int i;


    pid = getpid();
    while (1)
    {
        fprintf(stderr, "[%u] %lu   ", pid, count++);
        for (i=0; i<10; i++)
        {
            fprintf(stderr, "%c%c", 0x08, token[i & 0x3]);

            tv.tv_sec = 0;
            tv.tv_usec = 100000;
            select(0, NULL, NULL, NULL, &tv);
        }
        fprintf(stderr, "%c%c\n", 0x08, 0x20);
    }

    return 0;
}

