#include "associative_containers.hpp"

void set_operations()
{
	/*
	 * Template class implementing a container of unique objects of type Key, sorted using a comparison function. It uses a default compare function std::less, which can be changed via c-tor arg.
	 * O(log(n)) is the complexity of insertion, deletion and search.
	 * Implemented as red-black trees.
	 */ 
	 
	/*
	 * Iterators: bidirectional and const: begin, cbegin, end, cend
	 */  
	 
	/*
	 * Capacity: empty(), size()
	 */
	 
	/*
	 * Operations: clear(), erase(iterator)
	 * 				insert(key),insert(iterator hint, key), emplace(c-tor args), emplace_hint(iterator hint, value)
	 * 				swap(set))
	 */
	//insert returns pair <iterator of position where insertion took place, bool result>    
	std::set<int> iset;
	std::pair<std::set<int>::iterator, bool> result;
	result = iset.insert(1);
	std::cout<<"deref insert's result iterator: "<<*result.first<<" result bool value(if the insertion was successful) "<<result.second<<std::endl;
	result = iset.insert(1);
	std::cout<<"2nd attempt deref insert's result iterator: "<<*result.first<<" result bool value(if the insertion was successful) "<<result.second<<std::endl;
	
	//insert using initilizer list
	iset.insert({2,3});
	//emplace() useful to avoid calling copy or move c-tor, as object is constructed in place
	iset.emplace(4);
	//emplace_hint() with bad and better hint
	iset.emplace_hint(++iset.begin(), 5);
	iset.emplace_hint(--iset.end(), 6);
	
	std::cout<<"insert, insert initilizer list, emplace, emplace_hint: "<<std::endl;
	for(std::set<int>::iterator it = iset.begin(), end = iset.end(); it!=end; ++it)
		std::cout<<*it<<" ";
	std::cout<<"\n";
	/*
	 * Lookup: count(key) - returns # of elements having specified key
	 * 			find(key) - return iterator to element having key
	 * 			equal_range(key) - returns pair of iterators
	 * 			lower_bound(key), upper_bound(key) - return iterator to position of elements
	 */ 
	//counts # of elements that have the given key. Since no duplicates are allowed it returns 0 or 1.
	std::set<int>::size_type count = iset.count(5);
	std::cout<<"count: "<<count<<std::endl;
	
	//find address for a given key in the set
	std::set<int>::iterator find = iset.find(3);
	std::cout<<"find: "<<*find<<std::endl;
	
	//use iterators to find lower bound (the address of the key equal to or less than) and upper bound (address of the next key) for the given key
	std::set<int>::iterator lb = iset.lower_bound(3), ub = iset.upper_bound(3);
	std::cout<<"lower bound "<<*lb<<" upper bound "<<*ub<<std::endl;
	//use equal range to take lower and upper bounds
	std::pair<std::set<int>::iterator, std::set<int>::iterator> range = iset.equal_range(3);
	std::cout<<"lower and upper bounds using equal_range: "<<*(range.first)<<" "<<*(std::get<1>(range))<<std::endl; 
}
