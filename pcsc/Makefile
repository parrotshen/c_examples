CC = gcc
CFLAGS = -Wall -I/usr/include/PCSC
LDFLAGS = -lpcsclite

all: usim

usim: usim.o smartcard.o
	$(CC) $^ $(LDFLAGS) -o $@

usim.o: usim.c smartcard.h
	$(CC) -c $< $(CFLAGS) -o $@

smartcard.o: smartcard.c smartcard.h
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f usim *.o

.PHONY: all clean usim
