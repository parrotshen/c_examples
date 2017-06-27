CC = /usr/bin/gcc
AR = /usr/bin/ar
LD = /usr/bin/ld


all: libarchive.a main

main: main.o
	$(CC) -Wall -o $@ main.o libarchive.a

main.o: main.c archive.h
	$(CC) -Wall -c $<


## build archive ##
libarchive.a: hello.o world.o
	$(AR) rcs $@ $^

hello.o: hello.c
	$(CC) -Wall -c $<

world.o: world.c
	$(CC) -Wall -c $<


strip:
	strip main

clean:
	rm -f libarchive.a main *.o

