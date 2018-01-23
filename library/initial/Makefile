CC = gcc
AR = ar
LD = ld

PWD_DIR = $(shell /bin/pwd)


all: libinitial.so main

## shared library linked, you should copy libshared.so
## to /lib or other lib directory before you execute ./main
main: main.o
	$(CC) -Wall -o $@ main.o -L. -lshared  # -L specify the library directory

main.o: main.c initial.h
	$(CC) -Wall -c $<


## build share library ##
libinitial.so: initial.o
	$(CC) -shared -Wall -o $@ $^

initial.o: initial.c
	$(CC) -fPIC -Wall -c $<  # Position Independent Code

clean:
	rm -f libinitial.so
	rm -f main *.o

