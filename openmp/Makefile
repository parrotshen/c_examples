CC = gcc

all: parallel_for parallel_section parallel_single

parallel_for: parallel_for.c
	$(CC) -o $@ -std=gnu99 -Wall -fopenmp $<

parallel_section: parallel_section.c
	$(CC) -o $@ -std=gnu99 -Wall -fopenmp $<

parallel_single: parallel_single.c
	$(CC) -o $@ -std=gnu99 -Wall -fopenmp $<

clean:
	rm -f parallel_for parallel_section parallel_single
