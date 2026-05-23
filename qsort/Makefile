CC = gcc

all: qsort_int qsort_double qsort_string

qsort_int: qsort_int.c
	$(CC) $< -Wall -o $@

qsort_double: qsort_double.c
	$(CC) $< -Wall -o $@

qsort_string: qsort_string.c
	$(CC) $< -Wall -o $@

clean:
	rm -f qsort_int qsort_double qsort_string
