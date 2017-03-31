CC = gcc
CFLAGS = -Wall

all: color control

color: color.c
	$(CC) $(CFLAGS) $< -o $@

control: control.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f color control
