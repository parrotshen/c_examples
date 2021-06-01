CC = gcc

all: sysinfo lsps lsnet

sysinfo: sysinfo.c
	$(CC) -Wall sysinfo.c -o sysinfo

lsps: lsps.c
	$(CC) -Wall lsps.c -o lsps

lsnet: lsnet.c
	$(CC) -Wall lsnet.c -o lsnet

clean:
	rm -f sysinfo lsps lsnet
