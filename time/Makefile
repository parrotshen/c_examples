CC = gcc
CFLAGS = -Wall

all: time asctime

time: time.c
	$(CC) $(CFLAGS) $< -o $@

asctime: asctime.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f time asctime
