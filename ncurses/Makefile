CC = gcc
CFLAGS = -Wall -lncurses

all: my_ncurses

my_ncurses: my_ncurses.c
	$(CC) $< $(CFLAGS) -o $@

clean:
	rm -f my_ncurses
