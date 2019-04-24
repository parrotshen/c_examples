CC = gcc


all: buffer_overflow free_memory_error segment_fault

buffer_overflow: buffer_overflow.c
	$(CC) -Wall $< -o $@

free_memory_error: free_memory_error.c
	$(CC) -Wall $< -o $@

segment_fault: segment_fault.c
	$(CC) -Wall -rdynamic $< -o $@


clean:
	rm -f buffer_overflow free_memory_error segment_fault
