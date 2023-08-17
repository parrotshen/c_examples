CC = gcc

APP_NAME = mat_parser


all: $(APP_NAME)
	@strip $^

mat_parser: mat_parser.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -Wall -c $<

clean:
	@rm -f $(APP_NAME) *.o

.PHONY: all clean
