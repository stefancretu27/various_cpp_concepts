#include <iostream>
#include <tuple>
#include <string>

int main()
{
	//attempt tu use pointer to int to point to const int value fails. POinter to int cannot point to const int
	const int val = 7;
	//points to an int, but cannot covert from const int * to int*
	//int* pval = &val;
	
	//change int value through pointer to int and cont pointer to int
	std::int64_t i{27};
	std::int64_t* const pci = &i;
	*pci = 8;
	std::cout<<"int value change through a const pointer to int "<<*pci<<" "<<i<<std::endl;
	std::int64_t* pi = &i;
	*pi = 19;
	std::cout<<"int value change through a pointer to int "<<*pi<<" "<<i<<std::endl;
	
	//attempt to change int value via pointer to const int fails: assignment of read-only location. But pointer to const int can point to a non-const int.
	const std::int64_t* cpi = &i;	
	//*cpi = 271;
	
	//a reference acts like a const pointer: it must be initialized upon creation and references (points to) a single variable, thus cannot be reassigned
	std::int64_t &iref = i;
	iref = 3;
	std::cout<<"int value change through a ref "<<iref<<" "<<i<<std::endl;
	//use ref to const to reference non-const int, so the referenced value cannot be changed trough it
	const std::int64_t &ciref = i;
	
	//reference to const can reference rvalues
	const float &cfref{3.14 + 2.2};
 
	
	return 0;
}
