CC = gcc
CFLAGS = -Wall

all: factorial random shift

factorial: factorial.c
	$(CC) $(CFLAGS) $< -o $@

random: random.c
	$(CC) $(CFLAGS) $< -o $@

shift: shift.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f factorial random shift
