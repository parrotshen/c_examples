
all: test

Foo.o: Foo.cpp Foo.h
	g++ -Wall -c Foo.cpp -o Foo.o

FooWrapper.o: FooWrapper.cpp FooWrapper.h Foo.h
	g++ -Wall -c FooWrapper.cpp -o FooWrapper.o

main.o: main.c FooWrapper.h
	gcc -Wall -c main.c -o main.o

test: main.o FooWrapper.o Foo.o
	g++ main.o FooWrapper.o Foo.o -o test

clean:
	rm -f test *.o

.PHONY: all clean
