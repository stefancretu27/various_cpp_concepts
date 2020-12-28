#include "container_adaptors.hpp"

//generic function to print all types of stacks
template <typename T>
void print_queue(T &q)
{
	std::cout<<"print queue"<<std::endl;
	//modifiers: remove the first inserted element using pop
	while(!q.empty())
	{
		std::cout<<q.front()<<" ";
		q.pop();
	}
	std::cout<<std::endl;
}

void queue_operations()
{
	//use default deque underlying container
	std::queue<int> qi;
	
	/*
	 * Modifiers: push(), emplace(), pop()
	 */ 
	qi.push(7);
	qi.push(8);
	qi.emplace(3);
	qi.emplace(9);
	
	/*
	 * Capacity: empty(), size()
	 */ 
	std::cout<<"queue empty: "<<qi.empty()<<" size: "<<qi.size()<<std::endl;
	
	/*
	 * element access: only the first element front(), back()
	 */ 
	std::cout<<"queue front: "<<qi.front()<<" back: "<<qi.back()<<std::endl;
	
	print_queue(qi);
	
	/*
	 * Template also takes as arguments a sequence container used internally by the priority queue to store the elements. It has to provide front(), push_back and pop_back() methods.
	 * So, list, vector and deque seem suitable but list cannot be used as it does not have random access iterator such as vector's and deque's
	 * Array can not be used as push_back and pop_back are not implemented for it. The priority queue's push has to overlap a method in the container's class.
	 */
	
	std::queue<int, std::list<int>> qli;
	
	for(int n :{3,1,7,4,9,5})
		qli.push(n);
	
	print_queue(qli);
	

	
}
