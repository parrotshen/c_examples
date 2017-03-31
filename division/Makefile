CC = gcc
CFLAGS = -Wall

all: div_64bits div_negative

div_64bits: div_64bits.c
	$(CC) $(CFLAGS) $< -o $@

div_negative: div_negative.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f div_64bits div_negative
