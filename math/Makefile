CC = gcc
CFLAGS = -Wall

APPS += complex digit swap
APPS += euler pi prime random
APPS += remainder gcd_lcm factorize factorial
APPS += powxy logx nthroot
APPS += sinewave
APPS += rounding

all: $(APPS)

complex: complex.c
	$(CC) $< $(CFLAGS) -lm -o $@

digit: digit.c
	$(CC) $< $(CFLAGS) -o $@

swap: swap.c
	$(CC) $< $(CFLAGS) -o $@

euler: euler.c
	$(CC) $< $(CFLAGS) -lm -o $@

pi: pi.c
	$(CC) $< $(CFLAGS) -o $@

prime: prime.c
	$(CC) $< $(CFLAGS) -lm -o $@

random: random.c
	$(CC) $< $(CFLAGS) -o $@

remainder: remainder.c
	$(CC) $< $(CFLAGS) -lm -o $@

gcd_lcm: gcd_lcm.c
	$(CC) $< $(CFLAGS) -o $@

factorize: factorize.c
	$(CC) $< $(CFLAGS) -o $@

factorial: factorial.c
	$(CC) $< $(CFLAGS) -o $@

powxy: powxy.c
	$(CC) $< $(CFLAGS) -lm -o $@

logx: logx.c
	$(CC) $< $(CFLAGS) -lm -o $@

nthroot: nthroot.c
	$(CC) $< $(CFLAGS) -lm -o $@

sinewave: sinewave.c
	$(CC) $< $(CFLAGS) -lm -o $@

rounding: rounding.c
	$(CC) $< $(CFLAGS) -o $@

clean:
	rm -f $(APPS)
