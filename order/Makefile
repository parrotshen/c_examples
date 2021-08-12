CC = gcc
CFLAGS = -Wall

APPS = hton ntoh

all: $(APPS)

hton: hton.c
	$(CC) $(CFLAGS) -o $@ $<

ntoh: ntoh.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(APPS)
