CC = gcc
CFLAGS = -Wall
LDFLAGS = 

APPS  = test_set_value test_get_value test_set_data test_get_data
APPS += test_bitmap

all: $(APPS)

test_set_value: test_set_value.o bitwise.o
	$(CC) $(LDFLAGS) $^ -o $@

test_get_value: test_get_value.o bitwise.o
	$(CC) $(LDFLAGS) $^ -o $@

test_set_data: test_set_data.o bitwise.o
	$(CC) $(LDFLAGS) $^ -o $@

test_get_data: test_get_data.o bitwise.o
	$(CC) $(LDFLAGS) $^ -o $@

test_bitmap: test_bitmap.o bitmap.o bitwise.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c bitwise.h bitmap.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(APPS) *.o
