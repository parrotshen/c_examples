CC = gcc

all: arg_number arg_format arg_print

arg_number: arg_number.c
	$(CC) -Wall $< -o $@

arg_format: arg_format.c
	$(CC) -Wall $< -o $@

arg_print: arg_print.c
	$(CC) -Wall $< -o $@

clean:
	rm -f arg_number arg_format arg_print
