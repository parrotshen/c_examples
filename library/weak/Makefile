STRONG_SYMBOL = 1

CC = gcc


all: libweak.so main


main: main.o
	$(CC) -L. -lweak -o $@ $<

main.o: main.c weak.h
	$(CC) -Wall -D__STRONG_SYMBOL__=$(STRONG_SYMBOL) -c $<


libweak.so: weak.o
	$(CC) -shared -o $@ $<

weak.o: weak.c weak.h
	$(CC) -fPIC -Wall -c $<


clean:
	rm -f libweak.so
	rm -f main *.o

