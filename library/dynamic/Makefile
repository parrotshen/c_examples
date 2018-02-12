CC = gcc
AR = ar
LD = ld


all: libshared.so.1.0.1 main

main: main.c
	$(CC) -Wall -c main.c
	$(CC) -ldl -o main main.o


## build share library ##
libshared.so.1.0.1: hello.o world.o
	$(CC) -shared -Wl,-soname,libshared.so.1 -o $@ $^
	ln -sf libshared.so.1.0.1 libshared.so.1
	ln -sf libshared.so.1     libshared.so

hello.o: hello.c
	$(CC) -fPIC -Wall -c $<  # Position Independent Code

world.o: world.c
	$(CC) -fPIC -Wall -c $<  # Position Independent Code


clean:
	rm -f libshared.so libshared.so.1 libshared.so.1.0.1
	rm -f main *.o

