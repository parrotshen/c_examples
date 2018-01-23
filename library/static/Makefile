CC = gcc
AR = ar
LD = ld

PWD_DIR = $(shell /bin/pwd)


all: lib main

main: main.o
	$(CC) -Wall -o $@ main.o -L. -static -lstatic

main.o: main.c static.h
	$(CC) -Wall -c $<


lib: libstatic.so.1.0.1 libstatic.a

## build share library ##
libstatic.so.1.0.1: hello.o world.o
	$(CC) -shared -Wall -o $@ $^
	ln -sf libstatic.so.1.0.1 libstatic.so.1
	ln -sf libstatic.so.1     libstatic.so

## build archive ##
libstatic.a: hello.o world.o
	$(AR) rcs $@ $^

hello.o: hello.c
	$(CC) -fPIC -Wall -c $<  # Position Independent Code

world.o: world.c
	$(CC) -fPIC -Wall -c $<  # Position Independent Code


strip:
	strip main libstatic.so.1.0.1 libstatic.a

clean:
	rm -f libstatic.so libstatic.so.1 libstatic.so.1.0.1
	rm -f libstatic.a
	rm -f main *.o

