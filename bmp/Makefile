CC = gcc

all: test8 test16 test24

test8: test8.o bmp.o
	$(CC) $^ -o $@

test16: test16.o bmp.o
	$(CC) $^ -o $@

test24: test24.o bmp.o
	$(CC) $^ -o $@

%.o: %.c bmp.h
	$(CC) -Wall -c $< -o $@

clean:
	rm -f test8 test16 test24 *.o
