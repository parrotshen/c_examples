CC = gcc
CFLAGS = -Wall

all: buffer_overflow free_memory_error

buffer_overflow: buffer_overflow.c
	$(CC) $(CFLAGS) $< -o $@

free_memory_error: free_memory_error.c
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -f buffer_overflow free_memory_error
