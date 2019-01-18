CC = gcc
CFLAGS = -Wall

APPS = multicast_send multicast_recv mgroup

all: $(APPS)

multicast_send: multicast_send.o
	$(CC) $(CFLAGS) $< -o $@

multicast_recv: multicast_recv.o
	$(CC) $(CFLAGS) $< -o $@

mgroup: mgroup.o
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	\rm $(APPS) *.o
