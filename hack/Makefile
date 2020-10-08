CC = gcc
AS = as
LD = ld
OBJDUMP = objdump

CFLAGS = -Wall -m32 -g
LDFLAGS = -z execstack


all: buffer_overflow_1 buffer_overflow_2 shell_code goodbye exec_sh

buffer_overflow_1: buffer_overflow_1.o
	@echo "build ... $@"
	@$(CC) $(LDFLAGS) $< -o $@
	@$(OBJDUMP) -S $@ > $@.asm

buffer_overflow_2: buffer_overflow_2.o
	@echo "build ... $@"
	@$(CC) $(LDFLAGS) $< -o $@
	@$(OBJDUMP) -S $@ > $@.asm

shell_code: shell_code.o
	@echo "build ... $@"
	@$(CC) $(LDFLAGS) $< -o $@

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@


goodbye: goodbye.o
	@echo "build ... $@"
	@$(LD) -s -o $@ $<
	@$(OBJDUMP) -S $@ > $@.asm
	@./gen_shellcode_1.sh $@ > $@.code

exec_sh: exec_sh.o
	@echo "build ... $@"
	@$(LD) -s -o $@ $<
	@$(OBJDUMP) -S $@ > $@.asm
	@./gen_shellcode_1.sh $@ > $@.code

%.o: %.S
	@$(AS) -o $@ $<


clean:
	@echo "clean ..."
	@$(RM) buffer_overflow_1 buffer_overflow_2
	@$(RM) shell_code
	@$(RM) goodbye exec_sh
	@$(RM) *.o

.PHONY: all clean
