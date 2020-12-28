#include <iostream>
#include <tuple>
#include <string>

int main()
{
	//std::tuple
	//declare a tuple
	std::tuple<char, float, int> my_tuple;
	//set tuple's values'
	my_tuple = std::make_tuple('c', 0.1, 2);
	//use std::get template to get tuple values
	std::cout<<std::get<0>(my_tuple)<<" "<<std::get<1>(my_tuple)<<" "<<std::get<2>(my_tuple)<<std::endl;
	
	//create another tuple and define its values upon creation using uniform initialization
	std::tuple<int, std::string, float, double> tuple1{22, "string", 1.1, 3.14};
	//std::get can be used to set values, as well, as it returns a reference to the tuple's element
	std::get<1>(tuple1) = "asdf";
	std::cout<<std::get<0>(tuple1)<<" "<<std::get<1>(tuple1)<<" "<<std::get<2>(tuple1)<<" "<<std::get<3>(tuple1)<<std::endl;
	
	//use std::tuple_size to get # of elements in tuple
	std::cout<<"tuple size decltype "<<std::tuple_size<decltype(tuple1)>::value<<std::endl;
	
	//declare another tuple
	std::tuple<int, char> tuple2;
	char c = 'c';
	int i = 78;
	//use std::tie to add values to its elements. std::tie arguments are just references, not const references, so it does not accept lvalues
	tuple2 = std::tie(i, c);
	//print tuple values
	std::cout<<std::get<0>(tuple2)<<" "<<std::get<1>(tuple2)<<std::endl;
	
	char c2;
	int i2;
	//use std::tie to unpack tuple
	std::tie(i2,c2) = tuple2;
	
	//use std::tuple_element to get the type of the i-th element of the tuple
	std::tuple_element<3, decltype(tuple1)>::type double_val = 2.71;
	std::cout<<double_val<<std::endl;
	
	
	/* std::pair */
	std::cout<<"------------------pair part-----------------------"<<std::endl;
	//declare pair und use std::make_pair to initialize its values
	std::pair<int, char> pair;
	pair = std::make_pair(1, 'b');
	//print pair values using std::get and pair first/second fields, respectively
	std::cout<<std::get<0>(pair)<<" "<<std::get<1>(pair)<<std::endl;
	std::cout<<pair.first<<" "<<pair.second<<std::endl;
	
	//use uniform initialization
	std::pair<std::string, float> pair2{"string", 1.12};
	std::cout<<pair2.first<<" "<<pair2.second<<std::endl;
	//use pair first/second to change pair's values
	pair2.second = 78.91;
	std::cout<<pair2.first<<" "<<pair2.second<<std::endl;
	
	//use std::tie
	std::pair<int, double> pair3;
	int ii;
	double dd;
	//does not work to tie 2 lvalues to create pair
	//pair3 = std::tie{ii,dd};
	
	//but it works to untie a pair
	pair3 = std::make_pair(0, 0.0);
	std::tie(ii, dd) = pair3;
	std::cout<<ii<<" "<<dd<<std::endl;
	
	//use std::tuple_size
	std::cout<<"pair size using tuple_size+decltype "<<std::tuple_size<decltype(pair3)>::value<<std::endl;
	
	std::pair<int, double> pair4{1, 1.1};
	//swap 2 pairs
	pair3.swap(pair4);
	std::cout<<pair3.first<<" "<<pair3.second<<std::endl;
	return 0;
}
