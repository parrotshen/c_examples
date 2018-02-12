STRONG_SYMBOL = 0
WEAK_SYMBOL   = 0

CC = gcc


all: libweak.so main
	@echo "STRONG_SYMBOL = $(STRONG_SYMBOL)"
	@echo "WEAK_SYMBOL   = $(WEAK_SYMBOL)"


main: main.o
	$(CC) -L. -lweak -o $@ $<

main.o: main.c weak.h
	$(CC) -Wall -D__STRONG_SYMBOL__=$(STRONG_SYMBOL) -c $<


libweak.so: weak.o
	$(CC) -shared -o $@ $<

weak.o: weak.c weak.h
	$(CC) -fPIC -Wall -D__WEAK_SYMBOL__=$(WEAK_SYMBOL) -c $<


clean:
	rm -f libweak.so
	rm -f main *.o

