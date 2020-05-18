CC = gcc
CFLAGS = -Wall
LDFLAGS = 

APPS = test_set_value test_get_value test_set_data test_get_data

all: $(APPS)

test_set_value: test_set_value.o bitwise.o
	$(CC) $(LDFLAGS) $^ -o $@

test_get_value: test_get_value.o bitwise.o
	$(CC) $(LDFLAGS) $^ -o $@

test_set_data: test_set_data.o bitwise.o
	$(CC) $(LDFLAGS) $^ -o $@

test_get_data: test_get_data.o bitwise.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c bitwise.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(APPS) *.o
