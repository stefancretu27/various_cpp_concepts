CFLAGS = -std=c++14 -Wall -g
CC = g++

all: shared_ptr

shared_ptr: unique_ptr
	$(CC) $(CFLAGS) shared_ptr_cycle.cpp shared_ptr.cpp main.cpp -o shared_ptr.bin

unique_ptr:
	$(CC) $(CFLAGS) unique_ptr.cpp -o unique_ptr.bin

clean:
	rm *.bin
