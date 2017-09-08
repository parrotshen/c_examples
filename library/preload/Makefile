CC = /usr/bin/gcc
AR = /usr/bin/ar
LD = /usr/bin/ld


all: libpreload.so print time

print: print.o
	$(CC) -Wall -o $@ $^

print.o: print.c
	$(CC) -Wall -c $<

time: time.o
	$(CC) -Wall -o $@ $^

time.o: time.c
	$(CC) -Wall -c $<


## build shared library ##
libpreload.so: preload.o
	$(CC) -shared -Wall -o $@ $^

preload.o: preload.c
	$(CC) -fPIC -Wall -c $<


clean:
	rm -f libpreload.so
	rm -f print time *.o

