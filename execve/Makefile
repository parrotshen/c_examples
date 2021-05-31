CC = gcc -m32
LD = ld
AS = as
OBJDUMP = objdump
RM = rm -f


all: execve_demo execve_ls syscall_ls interpreter


# C program
execve_demo: execve_demo.o
	$(CC) -o $@ $<

execve_ls: execve_ls.o
	$(CC) -o $@ $<

interpreter: interpreter.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -Wall -c $< -o $@


# Assembly
syscall_ls: syscall_ls.o
	$(LD) -s -o $@ $<

%.o: %.S
	$(AS) -o $@ $<
	$(OBJDUMP) -S $@ > syscall_ls.asm


clean:
	$(RM) execve_demo execve_ls interpreter
	$(RM) syscall_ls syscall_ls.asm
	$(RM) *.o

.PHONY: all clean
