CFLAGS = -std=c++11 -Wall -g
CC = g++

all: lambda

lambda: functor
	$(CC) $(CFLAGS) lambda.cpp -o lambda.bin

functor:
	$(CC) $(CFLAGS) functor.cpp -o functor.bin

clean:
	rm *.bin
