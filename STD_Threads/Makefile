CFLAGS = -std=c++17 -g -Wall -pthread
CC = g++
SRCS = main.cpp mutex.cpp atomic.cpp promise_future.cpp
BIN = std_threads.bin

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN)

clean:
	rm -rf *.bin
