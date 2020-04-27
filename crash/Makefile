CC = gcc

APPS  = segment_fault
APPS += stack_overflow
APPS += buffer_overflow
APPS += bus_error
APPS += free_error
APPS += out_of_memory
APPS += divide_by_zero
APPS += variable_lifetime


all: $(APPS)

segment_fault: segment_fault.c
	$(CC) -Wall -rdynamic $< -o $@

stack_overflow: stack_overflow.c
	$(CC) -Wall $< -o $@

buffer_overflow: buffer_overflow.c
	$(CC) -Wall $< -o $@

bus_error: bus_error.c
	$(CC) -Wall $< -o $@

free_error: free_error.c
	$(CC) -Wall $< -o $@

out_of_memory: out_of_memory.c
	$(CC) -Wall $< -o $@

divide_by_zero: divide_by_zero.c
	$(CC) -Wall -g $< -o $@

variable_lifetime: variable_lifetime.c
	$(CC) -Wall -g $< -o $@

clean:
	rm -f $(APPS)
