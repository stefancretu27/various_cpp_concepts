CFLAGS = -std=c++14 -Wall -g
CC = g++

all: smart_ptrs

smart_ptrs:
	$(CC) $(CFLAGS) shared_ptr_cycle.cpp shared_ptr.cpp unique_ptr.cpp main.cpp -o smart_ptrs.bin

clean:
	rm *.bin
