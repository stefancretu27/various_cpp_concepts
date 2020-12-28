#include <iostream>
#include <string>

#include "Test.hpp"

int main()
{
	Test inst(1.1, 'd');
	int i = 77;
	std::string str("string test");

	//declare tuple
	std::tuple<Test, int, std::string> my_tuple;
	//make_tuple = create a tuple with values and store it in the above created tuple
	my_tuple = std::make_tuple(inst, i, str);
	//get = access tuple values
	std::cout<<"class double "<< std::get<0>(my_tuple).getD()<<" tuple int: "<<std::get<1>(my_tuple)<<" tuple str "<<std::get<2>(my_tuple)<<std::endl;
	
	std::cout<<"class double "<<std::get<0>(inst.getTuple())<<" class char "<<std::get<1>(inst.getTuple())<<std::endl;
	
	/*"tuple_size" is a helper template class of tuple class which returns the tuple's size at compile time. 
	* The templatized argument is the type of data which is retrieved using "decltype" (returns std::tuple)
	*"tuple_size" has a public static constant member "value" which is sizeof elements in tuple
	*/
	std::cout<<"tuple size "<<std::tuple_size<decltype(my_tuple)>::value<<" class tuple size "<<std::tuple_size<decltype(inst.getTuple())>::value<<std::endl;
	
	// swap - method of tuple class used for exchanging the values of the elements belonging to two tuples that pack same types in same order
	std::cout<<" before swapping tuples "<<std::endl;
	Test inst2(2.23, 'y');
	std::tuple<Test, int, std::string> second_tuple(inst2, 14, "second string");
	std::cout<<"class double "<< std::get<0>(my_tuple).getD()<<" tuple int: "<<std::get<1>(my_tuple)<<" tuple str "<<std::get<2>(my_tuple)<<std::endl;
	std::cout<<"class double "<< std::get<0>(second_tuple).getD()<<" tuple int: "<<std::get<1>(second_tuple)<<" tuple str "<<std::get<2>(second_tuple)<<std::endl;
	std::cout<<" after swapping tuples "<<std::endl;
	second_tuple.swap(my_tuple);
	std::cout<<"class double "<< std::get<0>(my_tuple).getD()<<" tuple int: "<<std::get<1>(my_tuple)<<" tuple str "<<std::get<2>(my_tuple)<<std::endl;
	std::cout<<"class double "<< std::get<0>(second_tuple).getD()<<" tuple int: "<<std::get<1>(second_tuple)<<" tuple str "<<std::get<2>(second_tuple)<<std::endl;
	
	//assignment operator
	std::cout<<" tuple 1 gets tuple 2 values tuples "<<std::endl;
	my_tuple = second_tuple;
	std::cout<<"class double "<< std::get<0>(my_tuple).getD()<<" tuple int: "<<std::get<1>(my_tuple)<<" tuple str "<<std::get<2>(my_tuple)<<std::endl;
	
	//tie = unpack tuple elements
	Test tres;
	int ires;
	std::string sres;
	
	tie(std::ignore, ires, sres) = second_tuple;
	std::cout<<" tuple int: "<<ires<<" tuple str "<<sres<<std::endl;
	
	tie(tres, ires, sres) = second_tuple;
	std::cout<<"class double "<< tres.getD()<<" tuple int: "<<ires<<" tuple str "<<sres<<std::endl;
	
	
	return 0;
}
