CC = gcc

all: arg_getopt arg_number arg_format arg_print arg_busybox

arg_getopt: arg_getopt.c
	$(CC) -Wall $< -o $@

arg_number: arg_number.c
	$(CC) -Wall $< -o $@

arg_format: arg_format.c
	$(CC) -Wall $< -o $@

arg_print: arg_print.c
	$(CC) -Wall $< -o $@

arg_busybox: arg_busybox.c
	$(CC) -Wall $< -o $@
	@ln -sf arg_busybox ps
	@ln -sf arg_busybox ls
	@ln -sf arg_busybox cp
	@ln -sf arg_busybox rm
	@ln -sf arg_busybox cat

clean:
	rm -f arg_getopt arg_number arg_format arg_print arg_busybox
	rm -f ps ls cp rm cat
