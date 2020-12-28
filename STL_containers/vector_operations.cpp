#include "sequence_containers.hpp"

void vector_operations()
{
	/*
	 * Template class that implements dynamic size arrays. That said, the storage is handled automatically, being shrinked or enlarged as needed.
	 * Thus, vectors occupy more space than arrays, as more memory is allocated for them in advance in order to reduce the number of memory allocations, which would be expensive. 
	 * The elements are stored contiguously, like in normal arrays
	 */
	 
	 //uniform initialization and iterate using for range loop
	 std::vector<std::string> strvec{"str", "str2", "str3"};
	 std::cout<<"for range loop: ";
	 for(std::string str: strvec)
		std::cout<<str<<" ";
	std::cout<<"\n";
	
	//copy initialization and iterate using iterators
	std::vector<std::int32_t> ivec = {0,1,2,3,4};
	std::cout<<"iterator loop: ";
	for(std::vector<std::int32_t>::iterator it = ivec.begin(), itend = ivec.end(); it!=itend; ++it)
		std::cout<<*it<<" ";
	std::cout<<"\n";
	
	/*
	 * Iterators: LegacyRandomAccessIterators: begin, end, rbegin, rend 
	 * 			  const LegacyRandomAccessIterators: cbegin, cend, crbegin, crend
	 */ 
	
	/*
	 * Element access: front(), back(), at(index) and indexing operator [index].
	 * As a consequence of having random access iterators.
	 */ 
	std::cout<<"front: "<<ivec.front()<<" back: "<<ivec.back()<<" at(index): "<<ivec.at(2)<<" indexing operator: "<<ivec[1]<<std::endl;
	
	//assign(#of values to assign, value)
	strvec.assign(4, "str4");
	
	/*
	 * Capacity: empty(), size(), capacity(), reserve(capacity amount), shrink_to_fit() which makes capacity=size
	 * size = #of elements existing; capacity = # of elements that can be stored in the allocated memory => size<=capacity
	 */
	std::cout<<"strvec size: "<<strvec.size()<<" max size: "<<strvec.max_size()<<" capacity: "<<strvec.capacity()<<std::endl;
	std::cout<<"ivec size: "<<ivec.size()<<" capacity: "<<ivec.capacity()<<std::endl;
	
	std::vector<float> fvec;
	//increase the capacity of vector. If the new capacity is less than or equal to the old one, it does not do anything
	fvec.reserve(5);
	fvec = {3.14, 2.73, -0.00999};
	std::cout<<"fvec size: "<<fvec.size()<<" capacity: "<<fvec.capacity()<<std::endl;
	//free unused memory
	fvec.shrink_to_fit();
	std::cout<<"after shrink_to_fit fvec size: "<<fvec.size()<<" capacity: "<<fvec.capacity()<<std::endl;
	std::vector<float> fvec2{1.1,2.2,3.3,4.4,5.5};
	fvec = fvec2;
	std::cout<<"after assigning a vector with more elements fvec size: "<<fvec.size()<<" capacity: "<<fvec.capacity()<<std::endl;
	
	/*
	 * Modifiers: 	clear(), erase(iterator or range of iterators), 
	 * 				push_back(value), emplace_back(c-tor args),
	 * 				insert(iterator, value or range of iterators), emplace(iterator, c-tor args),
	 * 				pop_back(),
	 * 				resize(# of values to resize to), swap(vector to swap with)
	 */
	//clears contents =>size=0, but capacity stays unchanged (no memory is cleared)
	fvec2.clear();
	std::cout<<"after clear fvec2 size: "<<fvec2.size()<<" capacity: "<<fvec2.capacity()<<std::endl;
	
	//insert(pos, value), insert value before pos
	fvec.insert(fvec.begin()+1, -273);
	std::cout<<"after insert fvec size: "<<fvec.size()<<" capacity: "<<fvec.capacity()<<std::endl;
	for(auto f:fvec)
		std::cout<<f<<" ";
	std::cout<<"\n";
	
	//erase(pos), erase(start it, end it), deletes element and reduces size, but capacity stays unchanged
	fvec.erase(fvec.cbegin(), fvec.cend());
	std::cout<<"after erase fvec size: "<<fvec.size()<<" capacity: "<<fvec.capacity()<<std::endl;
	
	//push_back(val): add element at the end and increase size
	fvec.push_back(3.14);
	fvec.push_back(1.34);
	std::cout<<"after 2xpush back fvec size: "<<fvec.size()<<" capacity: "<<fvec.capacity()<<std::endl;
	std::copy(fvec.begin(), fvec.end(), std::ostream_iterator<float>(std::cout, " "));
	std::cout<<"\n";
	
	//resize(count) resize to first count elements
	ivec.resize(3);
	for(std::vector<std::int32_t>::iterator it = ivec.begin(), itend = ivec.end(); it!=itend; ++it)
		std::cout<<*it<<" ";
	std::cout<<"\n";
	std::cout<<"after resize ivec size: "<<ivec.size()<<" capacity: "<<ivec.capacity()<<std::endl;
	
	//pop_back()
	ivec.pop_back();
		for(std::vector<std::int32_t>::iterator it = ivec.begin(), itend = ivec.end(); it!=itend; ++it)
		std::cout<<*it<<" ";
	std::cout<<"\n";
	std::cout<<"after pop_back ivec size: "<<ivec.size()<<" capacity: "<<ivec.capacity()<<std::endl;
	
	//emplace and emplace_back construct new element in place (in vector) using allocator
	ivec.emplace_back(44);
	std::cout<<"after emplace_back ivec size: "<<ivec.size()<<" capacity: "<<ivec.capacity()<<std::endl;
	std::copy(ivec.begin(), ivec.end(), std::ostream_iterator<int>(std::cout, " "));
	ivec.emplace(ivec.begin()+2, 567);
	std::cout<<std::endl<<"after emplace_back ivec size: "<<ivec.size()<<" capacity: "<<ivec.capacity()<<std::endl;
	std::copy(ivec.begin(), ivec.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
}
