#include "container_adaptors.hpp"

//generic function to print all types of priority queues
template <typename T>
void print_queue(T &q)
{
	std::cout<<"print priority queue"<<std::endl;
	//modifiers: remove first element using pop
	while(!q.empty())
	{
		std::cout<<q.top()<<" ";
		q.pop();
	}
	std::cout<<std::endl;
}

void priority_queue_operations()
{
	/*
	 * It is a queue that stores elements by sorting them in descending order, by default, so it provides constant time lookup to the largest element, which is always stored the first in the FIFO sequence.
	 * The insertion and data access complexities are logarithmic.
	 * Though the default ordering is descending, the user can provide a Compare function to change the ordering. Using std::greater<T> would sort elements in ascending order
	 */ 
	 
	std::priority_queue<int> pqi;
	
	/*
	 * Modifiers: push(), emplace() - both insert the element where it fist according to compare function. Thus, the queue remains sorted.
	 * 			pop()
	 */ 
	pqi.push(7);
	pqi.push(8);
	pqi.emplace(3);
	pqi.emplace(9);
	
	/*
	 * Capacity: empty(), size()
	 */ 
	std::cout<<"prio queue empty: "<<pqi.empty()<<" size: "<<pqi.size()<<std::endl;
	
	/*
	 * element access: only the first element top()
	 */ 
	std::cout<<"prio queue top: "<<pqi.top()<<std::endl;
	
	print_queue(pqi);
	
	/*
	 * Template also takes as arguments a sequence container used internally by the priority queue to store the elements. It has to provide front(), push_back and pop_back() methods.
	 * So, list, vector and deque seem suitable but list cannot be used as it does not have random access iterator such as vector's and deque's
	 * Array can not be used as push_back and pop_back are not implemented for it. The priority queue's push has to overlap a method in the container's class.
	 */
	
	std::priority_queue<int, std::deque<int>, std::greater<int>> pqdi;
	
	for(int n :{3,1,7,4,9,5})
		pqdi.push(n);
	
	print_queue(pqdi);
	
	auto lambda{[](int a, int b){ return a<b;}};
	std::priority_queue<int, std::vector<int>, decltype(lambda)> pqvi(lambda); 
	for(int n :{3,1,7,4,9,5})
		pqvi.push(n);
	
	print_queue(pqvi);
	
}
