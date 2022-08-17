CC = gcc


all: mmap
#	@echo "This is to test mmap() ..." > test.txt
#	@echo "Hello world !" >> test.txt
#	@echo >> test.txt
#	@echo "abcdefghijklmnopqrstuvwxyz" >> test.txt

mmap: mmap.c
	$(CC) -o $@ -Wall $<

clean:
	rm -f mmap
