#include "associative_containers.hpp"

void multiset_operations()
{
	/*
	 * Associatve container storing sorted objects of type Key, which are sorted using a Compare function.
	 * Unlike std::set, std::multiset allows for storing multiple object with the same value, that is more keys with the same value.
	 * Likewise std::set, search, insertion and deletion of a key have O(log(n)) complexity.
	 * Similar operations as set.
	 */ 
	 
	/*
	 * Iterators: bidirectional and const: begin, cbegin, end, cend
	 */  
	 
	/*
	 * Capacity: empty(), size()
	 */
	 
	/*
	 * Operations: clear(), erase(iterator)
	 * 				insert(key), insert(hint, key), emplace(c-tor args), emplace_hint(iterator pos, value)
	 * 				swap(set))
	 */ 
	 
	std::multiset<int> imset;
	
	//insert using initializer list
	imset.insert({5,1, 29, 12, 4, 5});
	//use iterators to print multiset
	std::cout<<"multiset built using initializer list of unordered values "<<std::endl;
	for(std::multiset<int>::iterator it = imset.begin(), end = imset.end(); it!=end; ++it)
		std::cout<<*it<<" ";
	std::cout<<"\n";
	
	//Unlike std::set, std::multiset.insert does not return a pair of iterator and bool, where bool tells if the insertion was successful, as multiple keys with similar values are allowed for being stored
	std::multiset<int>::iterator pos = imset.insert(2);
	std::cout<<"key "<<*pos<<" was inserted"<<std::endl;
	
	//Insert giving a hint, reusing the above defined iterator
	pos = imset.insert(++imset.begin(), 3);
	std::cout<<"key "<<*pos<<" was inserted using as hint postion of value: "<< *++imset.begin()<<std::endl;
	for(std::multiset<int>::iterator it = imset.begin(), end = imset.end(); it!=end; ++it)
		std::cout<<*it<<" ";
	std::cout<<"\n";
	
	std::multiset<int>::size_type sz= imset.size();
	//erase key in the given pos
	imset.erase(--imset.end());
	//erase key with the given value
	imset.erase(5);
	
	std::cout<<"multiset after erasing keys with value=5 and key on last position, which totals: "<<sz-imset.size()<<" elements "<<std::endl;
	for(std::multiset<int>::iterator it = imset.begin(), end = imset.end(); it!=end; ++it)
		std::cout<<*it<<" ";
	std::cout<<"\n";
	
	pos = imset.emplace(2);
	std::multiset<int>::iterator pos2 = imset.emplace_hint(--imset.end(), 6);
	std::cout<<"multiset after emplace and emplace hint of values "<<*pos<<" "<<*pos2<<std::endl;
	for(std::multiset<int>::iterator it = imset.begin(), end = imset.end(); it!=end; ++it)
		std::cout<<*it<<" ";
	std::cout<<"\n";
	
	/*
	 * Lookup: count(key) - returns # of elements having specified key
	 * 			find(key) - return iterator to element having key
	 * 			equal_range(key) - returns pair of iterators
	 * 			lower_bound(key), upper_bound(key) - return iterator to position of elements
	 */
	std::multiset<int>::size_type cnt = imset.count(2);
	std::cout<<"count # of keys=2 "<<cnt<<std::endl;
	
	std::multiset<int>::iterator find = imset.find(6);
	std::cout<<"key "<<*find<<" was found. It's prev is"<<*--find<<" it's next is: "<<*++++find<<std::endl;
	
	std::pair<std::multiset<int>::iterator, std::multiset<int>::iterator> range = imset.equal_range(2);
	std::cout<<" equal range for key 2. Lower bound key: "<<*range.first<<" upper bound key: "<<*range.second<<" prev of lower bound: "<<*--range.first<<std::endl;
	
	
	
}
