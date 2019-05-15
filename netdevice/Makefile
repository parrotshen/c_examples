CC = gcc

all: ifaddrs ifreq

ifaddrs: ifaddrs.o
	$(CC) $< -o $@

ifreq: ifreq.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) -Wall -c $< -o $@

clean:
	rm -f ifaddrs ifreq
	rm -f *.o
