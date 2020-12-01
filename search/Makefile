CC = gcc

all: linear_search binary_search interpolation_search fibonacci_search

linear_search: linear_search.c pattern.c
	$(CC) -Wall $< -o $@

binary_search: binary_search.c pattern.c
	$(CC) -Wall $< -o $@

interpolation_search: interpolation_search.c pattern.c
	$(CC) -Wall $< -o $@

fibonacci_search: fibonacci_search.c pattern.c
	$(CC) -Wall $< -o $@

clean:
	rm -f linear_search binary_search interpolation_search fibonacci_search
