CFLAGS = -std=c++14 -Wall -g
CC = g++
SRCS = Single_chain_inheritance/Root.cpp Single_chain_inheritance/Base.cpp Single_chain_inheritance/DerivedImpl.cpp virtuality.cpp main.cpp

all: virtuality

virtuality: 
	$(CC) $(CFLAGS) $(SRCS) -o virtuality.bin;

clean:
	rm *.bin
	cd Dump_class_hierarchy; rm *;

lang_class:
	$(CC) -fdump-lang-class $(SRCS) ;
	mv *.class *.out Dump_class_hierarchy
