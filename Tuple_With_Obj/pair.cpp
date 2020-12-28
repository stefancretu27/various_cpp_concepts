#include <iostream>
#include <utility>
#include <string>

#include "Test.hpp"

int main()
{
	Test inst(1.1, 'd');
	Test inst2(2.34, 'y');
	std::string str("string test");
	
	//declare and initialize pair using make_pair
	std::pair<Test, std::string> my_pair;
	my_pair = make_pair(inst, str);
	
	std::cout<<"first "<<my_pair.first.getC()<<" second "<<my_pair.second<<std::endl;
	
	// pair initialization through c-tor
	std::pair<Test, std::string> new_pair (inst2, "another one");
	std::cout<<"compare pairs"<<(my_pair==new_pair)<<std::endl;
	
	//pair gets default values if not initialized
	std::pair<Test, std::string> buff;
	std::cout<<"Pair def values first "<<buff.first.getC()<<" "<< buff.first.getD()<<"  second "<<buff.second<<std::endl;
	
	//swap pairs and use get like it is used for tuples to access pair element
	new_pair.swap(buff);
	std::cout<<"After swapping first "<<std::get<0>(buff).getC()<<" "<< buff.first.getD()<<"  second "<<buff.second<<std::endl;
	

	return 0;
}
