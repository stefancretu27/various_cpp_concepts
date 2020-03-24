CC = g++
DBG_FLAGS = -g -Wall
CFLAGS = -std=c++11

all:
	$(CC) $(CFLAGS) $(DBG_FLAGS) Node.cpp Queue.cpp main.cpp -o queue.exe

valgrind:
	valgrind --tool=memcheck ./queue.exe

clean:
	rm queue.exe;
