CFLAGS = -std=c++11 -Wall -g
CC = g++

all: move_semantics

move_semantics: shared_ptr_cycle
	$(CC) $(CFLAGS) move_semantics.cpp -o move_semantics.bin

shared_ptr_cycle:
	$(CC) $(CFLAGS) shared_ptr_cycle.cpp shared_ptr.cpp unique_ptr.cpp main.cpp -o shared_ptr_cycle.bin

clean:
	rm *.bin
