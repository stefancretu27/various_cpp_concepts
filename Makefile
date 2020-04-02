CFLAGS = -std=c++11 -Wall -g
CC = g++

all: shadowing

shadowing: template_method
	$(CC) $(CFLAGS) method_shadowing.cpp -o method_shadowing.bin

template_method:
	$(CC) $(CFLAGS) dtor_private.cpp -o template_method.bin

clean:
	rm *.bin
