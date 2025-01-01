CC = gcc

all: sysconf sysinfo rlimit rusage lsps lsnet gethostbyname getaddrinfo terminal

sysconf: sysconf.c
	$(CC) $< -Wall -o $@

sysinfo: sysinfo.c
	$(CC) $< -Wall -o $@

rlimit: rlimit.c
	$(CC) $< -Wall -o $@

rusage: rusage.c
	$(CC) $< -Wall -o $@

lsps: lsps.c
	$(CC) $< -Wall -o $@

lsnet: lsnet.c
	$(CC) $< -Wall -o $@

gethostbyname: gethostbyname.c
	$(CC) $< -Wall -o $@

getaddrinfo: getaddrinfo.c
	$(CC) $< -Wall -o $@

terminal: terminal.c
	$(CC) $< -Wall -o $@

clean:
	rm -f sysconf sysinfo rlimit rusage lsps lsnet gethostbyname getaddrinfo terminal
