CC = gcc


all: segment_fault buffer_overflow free_error out_of_memory

segment_fault: segment_fault.c
	$(CC) -Wall -rdynamic $< -o $@

buffer_overflow: buffer_overflow.c
	$(CC) -Wall $< -o $@

free_error: free_error.c
	$(CC) -Wall $< -o $@

out_of_memory: out_of_memory.c
	$(CC) -Wall $< -o $@


clean:
	rm -f segment_fault buffer_overflow free_error out_of_memory
