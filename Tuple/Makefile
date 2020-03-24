CC = g++
CFLAGS = -std=c++11 -g -Wall
TUPLE_SRC = tuple.cpp
TUPLE_EXE = tuple.exe
PAIR_SRC = pair.cpp
PAIR_EXE = pair.exe

all: tuple
	
tuple: pair
	$(CC) $(CFLAGS) $(TUPLE_SRC) -o $(TUPLE_EXE)
	
pair:
	$(CC) $(CFLAGS) $(PAIR_SRC) -o $(PAIR_EXE)
	
clean:
	rm *.exe
	
