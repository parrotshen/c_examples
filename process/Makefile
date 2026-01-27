CC = gcc

all: abort daemon zombie pthread system signal alarm \
     pause pipe backtrace ptrace ptrace_loop

abort: abort.c
	$(CC) -Wall $< -o $@

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
	rm -f abort daemon zombie pthread system signal alarm
	rm -f pause pipe backtrace ptrace ptrace_loop
