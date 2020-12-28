#include <iostream>
#include "globals_constants.hpp"

//pass constexpr as arg to const parameter
void func(const int& i)
{
    std::cout<<i<<std::endl;
    std::cout<<globals_constants::d<<std::endl;
}