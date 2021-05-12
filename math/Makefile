CC = gcc
CFLAGS = -Wall

APPS  = div_64bits div_negative
APPS += digit factorial pi random shift
APPS += double.ok double.fail

all: $(APPS)

double.ok: double.c
	$(CC) $(CFLAGS) -D__TOLERANCE__ -lm $< -o $@

double.fail: double.c
	$(CC) $(CFLAGS) $< -o $@

digit: digit.c
	$(CC) $(CFLAGS) $< -o $@

factorial: factorial.c
	$(CC) $(CFLAGS) $< -o $@

pi: pi.c
	$(CC) $(CFLAGS) $< -o $@

random: random.c
	$(CC) $(CFLAGS) $< -o $@

shift: shift.c
	$(CC) $(CFLAGS) $< -o $@

div_64bits: div_64bits.c
	$(CC) $(CFLAGS) $< -o $@

div_negative: div_negative.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(APPS)
