#include "container_adaptors.hpp"

//generic function to print all types of stacks
template <typename T>
void print_stack(T &q)
{
	std::cout<<"print stack"<<std::endl;
	//modifiers: remove the last inserted element using pop
	while(!q.empty())
	{
		std::cout<<q.top()<<" ";
		q.pop();
	}
	std::cout<<std::endl;
}

void stack_operations()
{
	//use default deque as underlying container 
	std::stack<int> si;
	
	/*
	 * Modifiers: push(), emplace(), pop()
	 */ 
	si.push(7);
	si.push(8);
	si.emplace(3);
	si.emplace(9);
	
	/*
	 * Capacity: empty(), size()
	 */ 
	std::cout<<"stack empty: "<<si.empty()<<" size: "<<si.size()<<std::endl;
	
	/*
	 * element access: only the first element top()
	 */ 
	std::cout<<"stack top: "<<si.top()<<std::endl;	
	print_stack(si);

	//use list as underlying container
	std::stack<int, std::list<int>> sli;
	for(int n :{3,1,7,4,9,5})
		sli.push(n);
	print_stack(sli);
	
	//use vector as underlying container
	std::stack<int, std::vector<int>> svi; 
	for(int n :{3,1,7,4,9,5})
		svi.push(n);
	print_stack(svi);
	
}
