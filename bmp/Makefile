CC = gcc

all: main

main: main.o bmp.o
	$(CC) $^ -o $@

%.o: %.c bmp.h
	$(CC) -Wall -c $< -o $@

clean:
	rm -f main *.o
