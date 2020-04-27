CC = gcc

APPS  = pointer_to_array
APPS += static_2d_array
APPS += dynamic_2d_array
APPS += array_redimension
APPS += call_by_value
APPS += call_by_reference

all: $(APPS)

pointer_to_array: pointer_to_array.c
	$(CC) -Wall $< -o $@

static_2d_array: static_2d_array.c
	$(CC) -Wall $< -o $@

dynamic_2d_array: dynamic_2d_array.c
	$(CC) -Wall $< -o $@

array_redimension: array_redimension.c
	$(CC) -Wall $< -o $@

call_by_value: call_by_value.c
	$(CC) -Wall $< -o $@

call_by_reference: call_by_reference.c
	$(CC) -Wall $< -o $@

clean:
	rm -f $(APPS)
