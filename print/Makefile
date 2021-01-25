CC = gcc

all: stdout stdout_flush stderr syslog terminal

stdout: stdout.c
	$(CC) -Wall -DPRINT_IMMEDIATELY=0 $< -o $@

stdout_flush: stdout.c
	$(CC) -Wall -DPRINT_IMMEDIATELY=1 $< -o $@

stderr: stderr.c
	$(CC) -Wall -DPRINT_IMMEDIATELY=0 $< -o $@

syslog: syslog.c
	$(CC) -Wall $< -o $@

terminal: terminal.c
	$(CC) -Wall $< -o $@

clean:
	rm -f stdout stdout_flush stderr syslog terminal
