CC = gcc

all: daemon zombie pthread system signal alarm pause \
     pipe backtrace ptrace ptrace_loop

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

alarm: alarm.c
	$(CC) -Wall $< -o $@

pause: pause.c
	$(CC) -Wall $< -o $@

pipe: pipe.c
	$(CC) -Wall $< -o $@

backtrace: backtrace.c
	$(CC) -Wall -rdynamic $< -o $@

ptrace: ptrace.c
	$(CC) -Wall $< -o $@

ptrace_loop: ptrace_loop.c
	$(CC) -Wall $< -o $@

clean:
	rm -f daemon zombie pthread system signal alarm pause
	rm -f pipe backtrace ptrace ptrace_loop
