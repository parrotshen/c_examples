CROSS = 
CC = $(CROSS)gcc
CFLAGS = -Wall

all: iic_test

iic_test: iic_test.c
	$(CC) $< $(CFLAGS) -o $@

clean:
	rm -f iic_test
