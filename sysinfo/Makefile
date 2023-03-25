CC = gcc

all: sysconf sysinfo rlimit rusage lsps lsnet

sysconf: sysconf.c
	$(CC) -Wall sysconf.c -o sysconf

sysinfo: sysinfo.c
	$(CC) -Wall sysinfo.c -o sysinfo

rlimit: rlimit.c
	$(CC) -Wall rlimit.c -o rlimit

rusage: rusage.c
	$(CC) -Wall rusage.c -o rusage

lsps: lsps.c
	$(CC) -Wall lsps.c -o lsps

lsnet: lsnet.c
	$(CC) -Wall lsnet.c -o lsnet

clean:
	rm -f sysconf sysinfo rlimit rusage lsps lsnet
