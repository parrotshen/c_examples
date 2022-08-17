CC = gcc

all: example predefined poundsign ambiguous vaargs

example: example.o
	$(CC) $< -o $@

predefined: predefined.o
	$(CC) $< -o $@

poundsign: poundsign.o
	$(CC) $< -o $@

ambiguous: ambiguous.o
	$(CC) $< -o $@

vaargs: vaargs.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) -Wall -c $< -o $@
	$(CC) -Wall -E $< > $(@:.o=.E.c)

clean:
	rm -f example predefined poundsign ambiguous vaargs
	rm -f *.o
