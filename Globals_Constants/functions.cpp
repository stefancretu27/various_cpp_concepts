#include <iostream>
#include "globals_constants.hpp"

/*
* The purpose of this function is to show that functions with const parameters can take constexpr arguments.
* Constexpr cannot be used as parameter type as the constexpr value must be specified upon initialization.
*/
void func(const int& i)
{
    std::cout<<"-------------------------------------------------------------------------------------------------------------------"<<std::endl;
	std::cout<<"Function accepting const int parameter and receiving constexpr arg. A parameter cannot be constexpr because it must be defined upon declaration"<<std::endl;
    std::cout<<i<<std::endl;
    std::cout<<globals_constants::d<<std::endl;
}
