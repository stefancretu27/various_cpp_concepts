#include "sequence_containers.hpp"

void array_operations()
{
	/*
	 * Template class that implements containers similar to C style arrays (like a struct encapsulating a statically allocated array)
	 * It's size is fixed and specified upon creation as a template argument
	 * Can be used as tuple of N elements of same size.
	 * It does not have modifiers. Looks like it is only used with data sets whose size is not to be modified during program's lifetime: no insert, delete, emplace, push, pop. Just update via assignment and indexing
	 */
	 
	 //construct using copy initialization
	 std::array<char, 2> a0 = {'j', 'k'};
	 //construct using uniform initialization
	 std::array<int, 5> a1{0,1,2,3,4};
	 
	/*
	 * Iterators: LegacyRandomAccessIterators: begin, end, rbegin, rend 
	 * 			  const LegacyRandomAccessIterators: cbegin, cend, crbegin, crend
	 */ 
	 
	/*
	 * Element access: front(), back(), at(index) and indexing operator [index]. 
	 * As consequence of having random access iterators
	 */ 
	std::cout<<"front()="<<a1.front()<<" back()="<<a1.back()<<" at(index)="<<a1.at(1)<<" index operator="<<a1[3]<<std::endl;  
	//get raw pointer to container
	int *parray =  a1.data();
	std::cout<<"print element using raw pointer to container "<<*(parray+2)<<std::endl<<std::endl;
	
	/*
	 * Capacity: empty(), size()
	 */ 
	std::cout<<"is array empty "<<a0.empty()<<std::endl;
	std::cout<<"array size "<<a0.size()<<std::endl;
	std::cout<<"max array size: max elements container can have due to system/library implementation "<<a0.max_size()<<std::endl;
	
	/*
	 * Operations: fill(data), swap(array_to_swap_with)
	 */ 
	//copy the arg character in all array positions
	a0.fill('w');
	//use std::copy from algorithm to copy the sequence of elements starting at first arg, ending at 2nd arg to destination starting at 3rd arg + using iterators
	std::copy(a0.begin(), a0.end(), std::ostream_iterator<char>(std::cout, " "));
	std::cout<<"\n";
	std::array<char, 2> a2{'q', 'p'};
	//swap 2 arrays
	a0.swap(a2);
	std::cout<<"a0 ";
	std::copy(a0.begin(), a0.end(), std::ostream_iterator<char>(std::cout, " "));
	std::cout<<std::endl<<"a2 ";
	for(char c: a2)
		std::cout<<c<<" ";
	
	/*
	 * Operators overloads
	 */
	std::cout<<std::endl<<"are arrays different "<<(a0!=a2)<<" are elements in 1st array smaller than those in 2nd array "<<(a0<a2)<<std::endl;
	
	//use reverse iterators
	std::cout<<" reversed array ";
	std::copy(a1.crbegin(), a1.crend(), std::ostream_iterator<int>(std::cout, " "));
	for(int &i:a1)
		i += 7;
	std::cout<<std::endl<<" array changed value in for range loop ";
	for(std::array<int,5>::const_iterator it = a1.cbegin(); it < a1.cend(); ++it)
		std::cout<<*it<<" ";
}
