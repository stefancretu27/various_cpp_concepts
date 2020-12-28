#include "sequence_containers.hpp"

void deque_operations()
{
	/*
	 * Template class that implements an indexable double ended queue that allows for fast insertion and deletions of elements at both of its ends.
	 * The deque's size (storage) is enlarged or shrinked as needed. Expansion is cheaper when compared to std::vector as the elements are not copied in the newly allocated memory.
	 * The deque's elements are not stored contiguously as in array and vector. Thus, pointer arithmetic with iterators is not supported.
	 * Instead, it uses fixed size arrays whose starting pointers are bookkept (array of pointers to arrays) which implies double dereference for element access.
	 */
	 
	//uniform initialization
	std::deque<float> fdeq{0.0, 1.1, -2.34567, 99.01};
	//copy initialization
	std::deque<int> ideq = {0,1,2,3,4,5,6};
	
	//print deque using iterator
	std::cout<<"print deque using interator"<<std::endl;
	for(std::deque<float>::iterator it = fdeq.begin(), end = fdeq.end(); it !=end; ++it)
		std::cout<<*it<<" ";
		
	/*
	 * Iterators: LegacyRandomAccessIterators: begin, end, rbegin, rend 
	 * 			  const LegacyRandomAccessIterators: cbegin, cend, crbegin, crend
	 */ 
	
	/*
	 * Element access: front(), back(), at(index) and indexing operator [index].
	 * As a consequence of having random access iterators.
	 */
	std::cout<<std::endl<<"print elements using front,back, at and indexing operator"<<std::endl;
	std::cout<<"front: "<<fdeq.front()<<" back: "<<fdeq.back()<<" fdeq.at: "<<fdeq.at(2)<<" [] "<<fdeq[1]<<std::endl;
	
	/*
	 * Capacity: size(), empty(), shrink_to_fit() - depends on implementation
	 */ 
	std::cout<<" is empty: "<<fdeq.empty()<<std::endl;
	std::cout<<" size: "<<fdeq.size()<<std::endl;
	
	/*
	 * Modifiers: clear(), erase(iterator or range of iterators)
	 * 				insert(iterator, value), emplace(iterator, c-tor args),
	 * 				push_front(value), emplace_front(c-tor args), push_back(value), emplace_back(c-tor args)
	 * 				pop_front(), pop_back(),
	 * 				resize(count), swap(deque to swap with)
	 */ 
	//clear() 
	fdeq.clear();
	std::cout<<"After clear(): is empty: "<<fdeq.empty()<<" size: "<<fdeq.size()<<std::endl;
	
	//push_front(value), emplace_front(value), push_back(value), emplace_back(value)
	fdeq.push_front(-23.3);
	fdeq.emplace_front(16.7);
	fdeq.push_back(2);
	fdeq.emplace_back(99.9);
	//emplace(iterator, value)
	fdeq.emplace(fdeq.begin()+2, -127);
	std::cout<<" push front, emplace front, push back, emplace back, emplace(2)"<<std::endl;
	std::copy(fdeq.begin(), fdeq.end(), std::ostream_iterator<float>(std::cout, " "));
	std::cout<<"\n";
	std::cout<<" size: "<<fdeq.size()<<std::endl;
	
	//erase(iterator)
	fdeq.erase(fdeq.begin()+2);
	//pop_back(), pop_front()
	fdeq.pop_back();
	fdeq.pop_front();
	std::cout<<"erase(it+2), pop back, pop front"<<std::endl;
	std::copy(fdeq.begin(), fdeq.end(), std::ostream_iterator<float>(std::cout, " "));
	std::cout<<"\n";
	std::cout<<" size: "<<fdeq.size()<<std::endl;
	
	//even if a larger iterator is provided, the newly element is allocated on the next position
	//insert(iterator, value)
	fdeq.insert(fdeq.begin()+7, 2);
	std::cout<<"insert(it+7)"<<std::endl;
	std::copy(fdeq.begin(), fdeq.end(), std::ostream_iterator<float>(std::cout, " "));
	std::cout<<"\n";
	std::cout<<"at(2) "<<fdeq.at(2)<<" size: "<<fdeq.size()<<std::endl;
	
}
