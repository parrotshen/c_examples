CC = gcc
AR = ar
LD = ld


all: libpreload.so print time

print: print.o
	$(CC) -o $@ $^

print.o: print.c
	$(CC) -Wall -c $<

time: time.o
	$(CC) -o $@ $^

time.o: time.c
	$(CC) -Wall -c $<


## build shared library ##
libpreload.so: preload.o
	$(CC) -shared -o $@ $^

preload.o: preload.c
	$(CC) -fPIC -Wall -c $<


clean:
	rm -f libpreload.so
	rm -f print time *.o

