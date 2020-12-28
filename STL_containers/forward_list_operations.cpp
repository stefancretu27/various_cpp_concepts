#include "sequence_containers.hpp"

void fwd_list_operations()
{
	/*
	 * Template class that implements a singly-linked list which allows for fast insertion and removal from anywhere in the list.
	 * Does not have an overhead when compared to C implementation. Likewise, elements are not stored contiguously.
	 * Does not support random access and does not allow bidirectional iteration as it only supports forward iterators.
	 */
	 
	 //uniform initialization
	 std::forward_list<int> ifwd_list{0,1,2,3,4,5}, ifwd_list2;
	 
	 //use iterator to print
	 std::cout<<"use iterator to print "<<std::endl;
	 for(std::forward_list<int>::iterator it = ifwd_list.begin(), end = ifwd_list.end(); it!=end; ++it)
		std::cout<<*it<<" ";
	std::cout<<"\n";
	
	/*
	 * Iterators: LegacyFrowardIterators and const: begin, cbegin, end, cend
	 * 			  before_begin, cbefore_begin - pointer to the element beofre the first. As it does not exist, accessing it causes seg fault but ised for insert_after(), emplace_after(), erase_after(), splice_after()
	 */ 
	
	/*
	 * Element access: front()
	 */
	/*
	 * Capacity: empty(), max_size()
	 */ 
	  
	std::cout<<"front: "<<ifwd_list.front()<<" empty "<<ifwd_list.empty()<<std::endl;
	
	/*
	 * Modifiers: clear(), erase_after(iterator)
	 * 			insert_after(iterator, value), emplace_after(iterator, c-tor args)
	 * 			push_front(value), emplace_front(c-tor args)
	 * 			pop_front()
	 * 			resize(#of elements), swap(forward_list to swap with)
	 */
	 ifwd_list2 = ifwd_list; 
	ifwd_list2.clear();
	std::cout<<"after clear() empty "<<ifwd_list2.empty()<<std::endl;

	//push_front(), emplace_front() 
	 ifwd_list.emplace_front(0);
	 ifwd_list.push_front(1);
	 
	 //pointer arithmetic with iterators supports only ++, as it performs incrementally ->next dereferencing
	 std::forward_list<int>::iterator bg = ++ifwd_list.begin();
	 //insert_after(), emplace_after() 
	 ifwd_list.insert_after(++bg, 2, 134); //2 copies
	 ifwd_list.emplace_after(++(++ifwd_list.begin()), 78);
	 
	 std::cout<<" emplace_front, push front, insert after, insert after(count), emplace_after"<<std::endl;
	 for(std::forward_list<int>::iterator it = ifwd_list.begin(), end = ifwd_list.end(); it!=end; ++it)
		std::cout<<*it<<" ";
	 std::cout<<"\n";
	
	/*
	 * Operations: sort(), reverse(), 
	 * 				merge(list2) - mergese 2 sorted lists, splice_after(iterator,list2, it2_begin, it2_end) = inserts values from range (it2_begin;it2_end) from list2 in list after iterator
	 * 				unique(value) - removes consecutive duplicates which equal to value
	 * 				remove(value), remove_if(condition or lambda)
	 */
	 //sort()
	std::cout<<"sort elements"<<std::endl;
	ifwd_list.sort();
	std::copy(ifwd_list.begin(), ifwd_list.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
	//reverse()
	std::cout<<"reverse elements"<<std::endl;
	ifwd_list.reverse();
	std::copy(ifwd_list.begin(), ifwd_list.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
	//unique()
	std::cout<<"remove consecutive duplicate elements"<<std::endl;
	ifwd_list.unique();
	std::copy(ifwd_list.begin(), ifwd_list.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
	
	ifwd_list.insert_after(++(++ifwd_list.begin()), {123,3356, 22, -9});
	std::cout<<"insert_after an initilizer list"<<std::endl;
	std::copy(ifwd_list.begin(), ifwd_list.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
	//remove()
	ifwd_list.remove(22);
	std::cout<<"remove all values=22"<<std::endl;
	std::copy(ifwd_list.begin(), ifwd_list.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
	//remove_if()
	ifwd_list.remove_if([](int elem){return elem>99; });
	std::cout<<"remove all >99"<<std::endl;
	std::copy(ifwd_list.begin(), ifwd_list.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
	
	//pop_front and resize
	ifwd_list.pop_front();
	std::cout<<"pop front"<<std::endl;
	std::copy(ifwd_list.begin(), ifwd_list.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
	ifwd_list.resize(7);
	std::cout<<"resize to 7 elements (more than it currently has)"<<std::endl;
	std::copy(ifwd_list.begin(), ifwd_list.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
	
	//splice_after() 
	std::forward_list<int> ifwd_list3{0,1,2,3,4,5};
	ifwd_list.splice_after(++ifwd_list.begin(), ifwd_list3, ++ifwd_list3.begin(), ifwd_list3.end());
	std::cout<<"splice after the 2nd element"<<std::endl;
	std::copy(ifwd_list.begin(), ifwd_list.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
	std::cout<<"2nd list"<<std::endl;
	std::copy(ifwd_list3.begin(), ifwd_list3.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<"\n";
	
	
}
