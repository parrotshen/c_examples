#include <stdio.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <error.h>

int main(void)
{
    struct sysinfo si;

    memset(&si, 0, sizeof(struct sysinfo));

    if (sysinfo( &si ) < 0)
    {
        perror( "sysinfo" );
        return -1;
    }

    printf("\n");
    printf("Seconds since boot .............. %ld\n", si.uptime);
    printf("1 minute load averages .......... %lu\n", si.loads[0]);
    printf("5 minutes load averages ......... %lu\n", si.loads[1]);
    printf("15 minutes load averages ........ %lu\n", si.loads[2]);
    printf("Total usable main memory size ... %lu (%lu MB)\n",
        si.totalram,
        ((si.totalram * si.mem_unit) >> 20));
    printf("Available memory size ........... %lu (%lu MB)\n",
        si.freeram,
        ((si.freeram * si.mem_unit) >> 20));
    printf("Amount of shared memory ......... %lu (%lu MB)\n",
         si.sharedram,
        ((si.sharedram * si.mem_unit) >> 20));
    printf("Memory used by buffers .......... %lu (%lu MB)\n",
         si.bufferram,
        ((si.bufferram * si.mem_unit) >> 20));
    printf("Total swap space size ........... %lu (%lu MB)\n",
         si.totalswap,
        ((si.totalswap * si.mem_unit) >> 20));
    printf("Swap space still available ...... %lu (%lu MB)\n",
         si.freeswap,
        ((si.freeswap * si.mem_unit) >> 20));
    printf("Number of current processes ..... %u\n", si.procs);
    printf("Total high memory size .......... %lu (%lu MB)\n",
         si.totalhigh,
        ((si.totalhigh * si.mem_unit) >> 20));
    printf("Available high memory size ...... %lu (%lu MB)\n",
         si.freehigh,
        ((si.freehigh * si.mem_unit) >> 20));
    printf("Memory unit size in bytes ....... %u\n", si.mem_unit);
    printf("\n");

    return 0;
}

