CC = gcc
CFLAGS = -Wall

all: snprintf strncpy sscanf strcasecmp strtok strcat strchr

snprintf: snprintf.c
	$(CC) $(CFLAGS) $< -o $@

strncpy: strncpy.c
	$(CC) $(CFLAGS) $< -o $@

sscanf: sscanf.c
	$(CC) $(CFLAGS) $< -o $@

strcasecmp: strcasecmp.c
	$(CC) $(CFLAGS) $< -o $@

strtok: strtok.c
	$(CC) $(CFLAGS) $< -o $@

strcat: strcat.c
	$(CC) $(CFLAGS) $< -o $@

strchr: strchr.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f snprintf strncpy sscanf strcasecmp strtok strcat strchr
