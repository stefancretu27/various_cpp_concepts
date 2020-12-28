CFLAGS = -std=c++11 -Wall -g
CC = g++
SEQ_CONTAINERS_SOURCES = array_operations.cpp vector_operations.cpp deque_operations.cpp forward_list_operations.cpp list_operations.cpp
ASSOC_CONTAINERS_SOURCES = set_operations.cpp multiset_operations.cpp map_operations.cpp multimap_operations.cpp
CONTAINER_ADAPTORS_SOURCES = stack_operations.cpp queue_operations.cpp priority_queue_operations.cpp

all:
	$(CC) $(CFLAGS) main.cpp $(SEQ_CONTAINERS_SOURCES) $(ASSOC_CONTAINERS_SOURCES) $(CONTAINER_ADAPTORS_SOURCES) -o containers.bin
	
clean:
	rm *.bin
