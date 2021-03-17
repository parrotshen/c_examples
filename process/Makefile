CC = gcc

all: daemon zombie pthread system signal pause pipe backtrace

daemon: daemon.c
	$(CC) -Wall $< -o $@

zombie: zombie.c
	$(CC) -Wall $< -o $@

pthread: pthread.c
	$(CC) -Wall -lpthread $< -o $@

system: system.c
	$(CC) -Wall $< -o $@

signal: signal.c
	$(CC) -Wall $< -o $@

pause: pause.c
	$(CC) -Wall $< -o $@

pipe: pipe.c
	$(CC) -Wall $< -o $@

backtrace: backtrace.c
	$(CC) -Wall -rdynamic $< -o $@

clean:
	rm -f daemon zombie pthread system signal pause pipe backtrace
