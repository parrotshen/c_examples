CC = gcc
CFLAGS = -Wall

all: time ctime gmtime localtime

time: time.c
	$(CC) $(CFLAGS) $< -o $@

ctime: ctime.c
	$(CC) $(CFLAGS) $< -o $@

gmtime: gmtime.c
	$(CC) $(CFLAGS) $< -o $@

localtime: localtime.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f time ctime gmtime localtime
