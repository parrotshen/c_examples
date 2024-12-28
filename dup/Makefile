CC = gcc

all: dup2_stdin dup2_stdout
	@cp template.txt dup2_stdin_test.txt
	@cp template.txt dup2_stdout_test.txt

dup2_stdin: dup2_stdin.c
	$(CC) $< -Wall -o $@

dup2_stdout: dup2_stdout.c
	$(CC) $< -Wall -o $@

clean:
	rm -f dup2_stdin dup2_stdout
	rm -f dup2_stdin_test.txt dup2_stdout_test.txt
