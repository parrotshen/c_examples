CC = gcc

all: root user

root: main.c
	sudo $(CC) -Wall $< -o $@
	sudo chmod u+s $@

user: main.c
	$(CC) -Wall $< -o $@

clean:
	sudo rm -f root
	rm -f user

.PHONY: all root user clean
