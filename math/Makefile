CC = gcc
CFLAGS = -Wall

APPS = factorial random shift div_64bits div_negative

all: $(APPS)

factorial: factorial.o
	$(CC) $(CFLAGS) $< -o $@

random: random.o
	$(CC) $(CFLAGS) $< -o $@

shift: shift.o
	$(CC) $(CFLAGS) $< -o $@

div_64bits: div_64bits.o
	$(CC) $(CFLAGS) $< -o $@

div_negative: div_negative.o
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(APPS) *.o
