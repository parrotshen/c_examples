CC = gcc
CFLAGS = -Wall

all: time asctime gmtime

time: time.c
	$(CC) $(CFLAGS) $< -o $@

asctime: asctime.c
	$(CC) $(CFLAGS) $< -o $@

gmtime: gmtime.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f time asctime gmtime
