CC = gcc
AR = ar
LD = ld


all: libarchive.a main

main: main.o
	$(CC) -o $@ main.o libarchive.a

main.o: main.c archive.h
	$(CC) -Wall -c $<


## build archive ##
libarchive.a: hello.o world.o
	$(AR) rcs $@ $^

hello.o: hello.c archive.h
	$(CC) -Wall -c $<

world.o: world.c archive.h
	$(CC) -Wall -c $<


strip:
	strip main

clean:
	rm -f libarchive.a main *.o

