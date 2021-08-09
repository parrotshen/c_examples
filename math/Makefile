CC = gcc
CFLAGS = -Wall

APPS  = div_64bits div_negative
APPS += digit swap
APPS += pi prime random
APPS += gcd_lcm factorize factorial
APPS += powxy logx

all: $(APPS)

digit: digit.c
	$(CC) $(CFLAGS) -o $@ $<

swap: swap.c
	$(CC) $(CFLAGS) -o $@ $<

pi: pi.c
	$(CC) $(CFLAGS) -o $@ $<

prime: prime.c
	$(CC) $(CFLAGS) -lm -o $@ $<

random: random.c
	$(CC) $(CFLAGS) -o $@ $<

gcd_lcm: gcd_lcm.c
	$(CC) $(CFLAGS) -o $@ $<

factorize: factorize.c
	$(CC) $(CFLAGS) -o $@ $<

factorial: factorial.c
	$(CC) $(CFLAGS) -o $@ $<

div_64bits: div_64bits.c
	$(CC) $(CFLAGS) -o $@ $<

div_negative: div_negative.c
	$(CC) $(CFLAGS) -o $@ $<

powxy: powxy.c
	$(CC) $(CFLAGS) -lm -o $@ $<

logx: logx.c
	$(CC) $(CFLAGS) -lm -o $@ $<

clean:
	rm -f $(APPS)
