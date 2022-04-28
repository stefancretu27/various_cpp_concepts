#pragma once
#include <iostream>
#include <type_traits>
using namespace std;

//dynamic polymorphism inheritance chain implementation
void Interface::printName()
{
    cout<<"Interface"<<endl;
};

class Derived1: virtual public Interface
{
    public:
    void printName() override
    {
        cout<<"Derived1"<<endl;
    }
};

class Derived2: virtual public Interface
{
    public:
    void printName() override
    {
        cout<<"Derived2"<<endl;
    }
};

class Derived12: public Derived1, public Derived2
{
    public:
    void printName() override
    {
        cout<<"Derived12"<<endl;
    }
};

//the above dynamic polymorphism prone inheritance chain is changed to the below
class Base1 
{
    public:
    void printName() const
    {
        cout<<"Base1"<<endl;
    }
};

class Base2
{
    public:
    void printName() const
    {
        cout<<"Base2"<<endl;
    }
};

class DerivedBase12: public Base1, public Base2
{
    public:
    //do not override non virtual methods
    void printName() const 
    {
        cout<<"DerivedBase12"<<endl;
    }
};

/*
* The interface used in the dynamic inheritance implementation is replaced with a template
* implementation whose type parameter is meant to be a placeholder for all types in the inheritance chain.
* Recommendation is to would be to encapsulate such interface functions into structs, thus allowing for both 
* full and partial specialization. Also, these methods' names would be more strongly typed. 
*/
template<typename T>
void printNameFunc(const T& entity)
{
    if(is_class<T>::value)
    {
        if(is_member_function_pointer<decltype(&T::printName)>::value)
        {
            entity.printName();
        }
    }
};
