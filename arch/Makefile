CROSS = 
CC = $(CROSS)gcc
NM = $(CROSS)nm
SIZE = $(CROSS)size
OBJDUMP = $(CROSS)objdump


all: hello test memory

hello: hello.o
	@echo "[BUILD] $@"
	$(CC) $< -m32 -o $@
	@echo "Generating hello.sym and hello.asm ..."
	$(OBJDUMP) -t hello | grep "\.text" | sort > hello.sym
	$(OBJDUMP) -S hello > hello.asm
	$(OBJDUMP) -f hello > hello.arch
	ldd $@
	@echo

hello.o: hello.c
	$(CC) -Wall -m32 -g -c $< -o $@
	$(NM) $@

test: test.c
	@echo "[BUILD] $@"
	$(CC) -Wall $< -o $@
	@echo
	@file $@
	@echo

memory: memory.c
	@echo "[BUILD] $@"
	$(CC) -Wall $< -o $@
	$(SIZE) $@
	@echo


clean:
	rm -f hello hello.o hello.sym hello.asm hello.arch
	rm -f test
	rm -f memory

.PHONY: hello test memory
