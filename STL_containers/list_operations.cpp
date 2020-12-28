#include "sequence_containers.hpp"

void list_operations()
{
	/* 
	 * Template class that implements a conatiner based on a doubly-linked list. It allows for pushing elements at both ends.
	 * Elements are not stored contiguously, thus it does not allow for random access
	 * Unlike forward list, it provides reverse iterator that allow for reverse traversial.
	 * Like forward list, insertion and deletion of an element are O(1), whilst search and access are O(n)
	 */ 
	 
	std::list<int> ilist{0,1,2,3,4,5};
	std::list<int> ilist2, ilist_bk;
	
	//assign value to container
	ilist2.assign(3,-3);
	
	/*
	 * Iterators: LegacyBidirectionalIterators and const - begin, cbegin, end, cend, rbegin, crbegin, rend, crend
	 */ 
	std::cout<<"reverse iterator to print list in reverse order"<<std::endl;
	//use reverse iterator
	for(std::list<int>::reverse_iterator it = ilist.rbegin(), rend = ilist.rend(); it!=rend; ++it)
		std::cout<<*it<<" ";
	std::cout<<"\n";
	
	/*
	 * Element access: front(), back()
	 */ 
	std::cout<<"font: "<<ilist.front()<<" back: "<<ilist.back()<<std::endl;
	
	/*
	 * Capacity: size(), empty()
	 */ 
	ilist2.clear();
	std::cout<<"After clear empty: "<<ilist2.empty()<<" size: "<<ilist2.size()<<std::endl;
	
	/*
	 * Modifiers: clear(), erase(iterator)
	 * 			  insert(iterator, value), emplace(iterator, c-tor args),
	 * 			  push_front(value), emplace_front(c-tor args), push_back(value), emplace_back(c-tor args)
	 * 			  pop_front(), pop_back()
	 * 			  resize(), swap()
	 */
	//insert(iterator, value or initilizer list)  
	ilist2.insert(ilist2.begin(), {33, 44});
	ilist2.insert(++(++ilist2.begin()), 3, 11);
	//emplace_front(), push_front()
	ilist2.emplace_front(77);
	ilist2.push_front(99);
	//emplace(), emplace_back(), push_back()
	ilist2.emplace(++ilist2.begin(), 88);
	ilist2.emplace_back(1909);
	ilist2.push_back(731);
	
	std::cout<<"insert initilizer list of 2 elements on empty list, then insert 3 elements at the end, then emplace_front, push_front,emplace on pos 1 and emplace_back"<<std::endl;
	std::copy(ilist2.begin(), ilist2.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl;
	
	//pop_front(), pop_back()
	ilist2.pop_front();
	ilist2.pop_back();
	//erase()
	ilist2.erase(++(++ilist2.begin()),  --(--ilist2.end()));
	std::cout<<"pop_front, pop_back, erase everything but first 2 and last 2"<<std::endl;
	std::copy(ilist2.begin(), ilist2.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl;
	
	/*
	 * Operations: sort(), reverse(), unique(value) - removes consecutive duplicates of value
	 * 				merge(list2) - merge 2 sorted lists into list, splice(iterator,list2, it2_begin, it2_end) = inserts values from range (it2_begin;it2_end) from list2 in list after iterator
	 * 				remove(value), remove_if(lambda condition)
	 */
	//sort()  
	ilist2.sort();
	std::cout<<"sort"<<std::endl;
	std::copy(ilist2.begin(), ilist2.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl;
	
	//merge()
	ilist_bk = ilist2;
	ilist.merge(ilist2);
	ilist2 = ilist_bk;
	std::cout<<"merge. The merged list becomes empty and the list to merge with becomes larger by the new list"<<std::endl;
	std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl;
	std::copy(ilist2.begin(), ilist2.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl;
	
	//splice()
	auto it = ilist.end();
	std::advance(it, -2);
	ilist.splice(it, ilist2, ++ilist2.begin(), --ilist2.end());
	std::cout<<"splice = moves elements from one list to another and advance"<<std::endl;
	std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl;
	std::copy(ilist2.begin(), ilist2.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl;
	
	ilist2.resize(5);
	std::copy(ilist2.begin(), ilist2.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl<<"size after resize:"<<ilist2.size()<<std::endl;
	
	//unique()
	ilist.insert(ilist.end(), {0, 1});
	ilist.unique();
	std::cout<<"unique = remove consecutive unique values"<<std::endl;
	std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl;
	
	//remove(), remove_if()
	ilist.remove(77);
	ilist.remove_if([](int v){return v>1000;});
	std::cout<<"remove 77 and remove_if > 1000"<<std::endl;
	std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl;
	
	//reverse()
	ilist.reverse();
	std::cout<<"reverse"<<std::endl;
	std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout<<std::endl;
	
	
	
}
