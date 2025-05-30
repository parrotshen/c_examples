CC = gcc

all: test16 test24

test16: test16.o bmp.o
	$(CC) $^ -o $@

test24: test24.o bmp.o
	$(CC) $^ -o $@

%.o: %.c bmp.h
	$(CC) -Wall -c $< -o $@

clean:
	rm -f test16 test24 *.o
