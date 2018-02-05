EXPORT_RESTRICT = 1

CC = gcc
AR = ar
LD = ld


all: libfoo.so libbar.so main

## shared library linked, you should copy libshared.so
## to /lib or other lib directory before you execute ./main
main: main.o
	$(CC) -L. -lfoo -lbar -o $@ $<

main.o: main.c foo.h bar.h
	$(CC) -Wall -c $<


## build share library ##
libfoo.so: foo.o
ifeq ($(EXPORT_RESTRICT), 1)
	$(CC) -shared -Wl,--version-script=libfoo.map -o $@ $<
else
	$(CC) -shared -o $@ $<
endif

libbar.so: bar.o
ifeq ($(EXPORT_RESTRICT), 1)
	$(CC) -shared -Wl,--version-script=libbar.map -o $@ $<
else
	$(CC) -shared -o $@ $<
endif

foo.o: foo.c
	$(CC) -fPIC -Wall -c $<

bar.o: bar.c
	$(CC) -fPIC -Wall -c $<


clean:
	rm -f libfoo.so libbar.so
	rm -f main *.o

