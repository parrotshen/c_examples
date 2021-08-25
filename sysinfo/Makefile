CC = gcc

all: sysinfo rlimit lsps lsnet

sysinfo: sysinfo.c
	$(CC) -Wall sysinfo.c -o sysinfo

rlimit: rlimit.c
	$(CC) -Wall rlimit.c -o rlimit

lsps: lsps.c
	$(CC) -Wall lsps.c -o lsps

lsnet: lsnet.c
	$(CC) -Wall lsnet.c -o lsnet

clean:
	rm -f sysinfo rlimit lsps lsnet
