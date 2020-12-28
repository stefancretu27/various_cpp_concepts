#include "sequence_containers.hpp"
#include "associative_containers.hpp"
#include "container_adaptors.hpp"

//#define SEQUENCE_CONTAINERS
//#define ASSOCIATIVE_CONTAINERS
#define CONTAINER_ADAPTORS

int main()
{
#ifdef SEQUENCE_CONTAINERS
	std::cout<<"----------------------------Array operations----------------------------"<<std::endl;
	array_operations();
	std::cout<<std::endl<<"----------------------------Vector operations----------------------------"<<std::endl;
	vector_operations();
	std::cout<<std::endl<<"----------------------------Double ended queue operations----------------------------"<<std::endl;
	deque_operations();
	std::cout<<std::endl<<"----------------------------Forward list operations----------------------------"<<std::endl;
	fwd_list_operations();
	std::cout<<std::endl<<"----------------------------List operations----------------------------"<<std::endl;
	list_operations();
#endif

#ifdef ASSOCIATIVE_CONTAINERS
	std::cout<<"----------------------------Set operations----------------------------"<<std::endl;
	set_operations();
	std::cout<<"----------------------------Multiset operations----------------------------"<<std::endl;
	multiset_operations();
	std::cout<<"----------------------------Map operations----------------------------"<<std::endl;
	map_operations();
	std::cout<<"----------------------------MultiMap operations----------------------------"<<std::endl;
	multimap_operations();
#endif

#ifdef CONTAINER_ADAPTORS
	std::cout<<"----------------------------Container adaptors operations----------------------------"<<std::endl;
	stack_operations();
	queue_operations();
	priority_queue_operations();
#endif		
	return 0;
}
