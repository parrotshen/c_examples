CC = gcc

all: viterbi27Dec convolutionEnc

viterbi27Dec: viterbi27Dec.c
	$(CC) $< -Wall -lfec -o $@

convolutionEnc: convolutionEnc.c
	$(CC) $< -Wall -o $@

clean:
	rm -f viterbi27Dec convolutionEnc
