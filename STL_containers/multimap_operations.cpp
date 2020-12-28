#include "associative_containers.hpp"

void multimap_operations()
{
	/*
	 * Associative container storing pairs <key, value> sorted by key, where more entries with same key are allowed.
	 * Implemented as red-black trees.
	 * Search, insertion and deletion of node has O(log(n)) complexity.
	 */ 
	
	/*
	 * Iterators: LegacyBidirectionalIterators and const - begin, cbegin, end, cend, rbegin, crbegin, rend, crend
	 */
	/*
	 * Capacity: empty(), size() - # of tuples
	 */   
	std::multimap<int,char> icmmap{{110, 'a'}, {22, 'y'}};
	
	/*
	 * Operations: clear(), erase(iterator)
	 * 				insert(key), insert(iterator hint, key), emplace(c-tor args), emplace_hint(iterator hint, value)
	 * 				swap(set))
	 */
	//insert() returns iterator
	std::multimap<int, char>::iterator itres = icmmap.insert({77, 'q'});
	std::cout<<"inserted: "<<itres->first<<" "<<itres->second<<std::endl;
	
	//insert using initializer list
	icmmap.insert({{9, 'q'}, {22, 'd'}});
	for(std::multimap<int, char>::iterator it = icmmap.begin(), end = icmmap.end(); it!=end; ++it )
		std::cout<<"key: "<<it->first<<" "<<it->second<<std::endl;
	std::cout<<"\n";
	
	//emplace()
	itres = icmmap.emplace(std::make_pair<int, char>(77, 'p'));
	std::cout<<"inserted: "<<itres->first<<" "<<itres->second<<std::endl;
	//emplace_hint() with insert existing pair =>successfull operation
	itres = icmmap.emplace_hint(--icmmap.end(), std::make_pair<int, char>(77, 'q'));
	std::cout<<"inserted: "<<itres->first<<" "<<itres->second<<std::endl;
	
	for(std::multimap<int, char>::iterator it = icmmap.begin(), end = icmmap.end(); it!=end; ++it )
		std::cout<<"key: "<<it->first<<" "<<it->second<<std::endl;
	std::cout<<"\n";
	
	/*
	 * Lookup: count(key) - returns # of elements having specified key
	 * 			find(key) - return iterator to element having key
	 * 			equal_range(key) - returns pair of iterators
	 * 			lower_bound(key), upper_bound(key) - return iterator to position of elements
	 */
	std::pair<std::multimap<int, char>::iterator, std::multimap<int, char>::iterator> pair = icmmap.equal_range(77);
	std::cout<<"lower bound value (less than or equal)) for key=77: "<<pair.first->second<<" upper bound (next value freater than): "<<pair.second->second<<std::endl;
	
	//erase()
	std::cout<<"delete last entry and entries with key=22"<<std::endl;
	icmmap.erase(--icmmap.end());
	icmmap.erase(22);
	
	for(std::multimap<int, char>::iterator it = icmmap.begin(), end = icmmap.end(); it!=end; ++it )
		std::cout<<"key: "<<it->first<<" "<<it->second<<std::endl;
	std::cout<<"\n";
	
}
