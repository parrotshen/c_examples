CC = gcc

all: queue_test producer_consumer

queue_test: queue_test.o queue.o
	$(CC) -lpthread $^ -o $@

producer_consumer: producer_consumer.o queue.o
	$(CC) -lpthread $^ -o $@

%.o: %.c queue.h
	$(CC) -Wall -c $< -o $@

clean:
	rm -f queue_test producer_consumer
	rm -f *.o
