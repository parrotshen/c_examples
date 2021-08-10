CC = gcc
CFLAGS = -Wall

APPS += complex digit swap
APPS += euler pi prime random
APPS += gcd_lcm factorize factorial
APPS += powxy logx

all: $(APPS)

complex: complex.c
	$(CC) $(CFLAGS) -o $@ $<

digit: digit.c
	$(CC) $(CFLAGS) -o $@ $<

swap: swap.c
	$(CC) $(CFLAGS) -o $@ $<

euler: euler.c
	$(CC) $(CFLAGS) -lm -o $@ $<

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

powxy: powxy.c
	$(CC) $(CFLAGS) -lm -o $@ $<

logx: logx.c
	$(CC) $(CFLAGS) -lm -o $@ $<

clean:
	rm -f $(APPS)
