#include "TemplateClass.hpp"

template<class T>
void CheckType<T>::printType() const
{
    std::cout<<"type is not pointer or reference"<<std::endl;
}

template<class T>
void CheckType<T*>::printType() const
{
    std::cout<<"type is pointer"<<std::endl;
}

template<class T>
void CheckType<T&>::printType() const
{
    std::cout<<"type is reference"<<std::endl;
}