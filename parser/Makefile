CC = gcc
CFLAGS = -Wall
LDFLAGS = -Wall

all: parser hex ipv4 ipv6

parser: parser.o utility.o
	$(CC) $(LDFLAGS) -o $@ $^

parser.o: parser.c utility.h
	$(CC) $(CFLAGS) -c $<

hex: hex.o utility.o
	$(CC) $(LDFLAGS) -o $@ $^

hex.o: hex.c utility.h
	$(CC) $(CFLAGS) -c $<

ipv4: ipv4.o utility.o
	$(CC) $(LDFLAGS) -o $@ $^

ipv4.o: ipv4.c utility.h
	$(CC) $(CFLAGS) -c $<

ipv6: ipv6.o utility.o
	$(CC) $(LDFLAGS) -o $@ $^

ipv6.o: ipv6.c utility.h
	$(CC) $(CFLAGS) -c $<

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f parser hex ipv4 ipv6
	rm -f *.o
