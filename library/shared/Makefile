CC = gcc
AR = ar
LD = ld

PWD_DIR = $(shell /bin/pwd)


all: libshared.so.1.0.1 main

## shared library linked, you should copy libshared.so
## to /lib or other lib directory before you execute ./main
main: main.o
	$(CC) -o $@ main.o -L. -lshared  # -L specify the library directory

main.o: main.c shared.h
	$(CC) -Wall -c $<


## build share library ##
libshared.so.1.0.1: hello.o world.o
	$(CC) -shared -Wl,-soname,libshared.so.1 -o $@ $^
	ln -sf libshared.so.1.0.1 libshared.so.1
	ln -sf libshared.so.1     libshared.so

hello.o: hello.c shared.h
	$(CC) -fPIC -Wall -c $<  # Position Independent Code

world.o: world.c shared.h
	$(CC) -fPIC -Wall -c $<  # Position Independent Code


strip:
	strip main libshared.so.1.0.1

clean:
	rm -f libshared.so libshared.so.1 libshared.so.1.0.1
	rm -f main *.o

