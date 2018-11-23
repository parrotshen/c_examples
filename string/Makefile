CC = gcc
CFLAGS = -Wall

all: snprintf sscanf strcasecmp strtok

snprintf: snprintf.c
	$(CC) $(CFLAGS) $< -o $@

sscanf: sscanf.c
	$(CC) $(CFLAGS) $< -o $@

strcasecmp: strcasecmp.c
	$(CC) $(CFLAGS) $< -o $@

strtok: strtok.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f snprintf sscanf strcasecmp strtok
