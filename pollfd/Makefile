CC = gcc

all: pollfd
	@test -f test.pipe || mkfifo test.pipe

pollfd: pollfd.c
	$(CC) -Wall -o $@ $<

clean:
	rm -f pollfd test.pipe
