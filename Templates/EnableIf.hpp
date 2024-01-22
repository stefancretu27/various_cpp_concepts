#include <type_traits>
#include <iostream>
#include <typeinfo>
	
/*
* template< bool B, class T = void > struct enable_if is a template struct defined in type_traits header.
* Based on predicate B's value, it internally typedefs a "type" member, if true. The type is an alias for the data type
* passed as the second template parameter T. A possible implementation is shown below:
* 
* //base template definition
* template<bool B, class T = void>
* struct enable_if {};
* 
* //partial specialization for B = true
* template<class T>
* struct enable_if<true, T> { typedef T type; };
* 
* The behavior of a program that adds specializations for std::enable_if is undefined.
*
* enable_if can be used as:
*    - return type in a template function,
*    - template parameter for classes/functions,
*    - additional function argument.
*    - inside a template class or function, can be used for a variable.s or memebr's data type
*/

//used as non-type template parameter, which or_equires a default value, if the type is enabled
template<class T, typename std::enable_if<std::is_integral<T>::value /*The B predicate*/, bool /* T=bool type, defaulted to void*/>::type = true>
class IntegersWrapper
{
    public:
    IntegersWrapper(T&& t):intData{t}{};
    T GetData() const {return intData;};
    void PrintData() const
    {
        std::cout<<intData<<std::endl;
    }
    void PrintType() const
    {
        typename std::enable_if<std::is_trivially_constructible<T>::value, bool>::type someData;
        std::cout<<"typeid "<<typeid(someData).name()<<" "<<(someData==false)<<std::endl;
    }
    
    private:
    T intData;
};

//used as type template parameter, as the typename receives the returned type, if enabled
template<class T, typename = std::enable_if<std::is_floating_point<T>::value, void>>
class DoubleWrapper
{
    public:
    DoubleWrapper(T&& t):intData{t}{};
    T GetData() const {return intData;};
    void PrintData() const
    {
        std::cout<<intData<<std::endl;
    }
    void PrintType() const
    {
        typename std::enable_if<std::is_trivially_constructible<T>::value, bool>::type someData;
        std::cout<<"typeid "<<typeid(someData).name()<<" "<<(someData==false)<<std::endl;
    }
    
    private:
    T intData;
};

//used as function return type, as the data type is both enabled via the return type, but also specifies the return type
template<class T>
typename std::enable_if<std::is_trivially_default_constructible<T>::value, T>::type
EnableViaReturnType(T&& t)
{
    std::cout<<__FUNCTION__<<" "<<t<<" is default constructible and trivial type "<<std::endl;
    return t;
}

template<class T>
void EnableViaFunctionParameter(T&& t, typename std::enable_if<std::is_trivially_default_constructible<T>::value, bool>::type = true)
{
    std::cout<<__FUNCTION__<<" "<<t<<" is default constructible and trivial type "<<std::endl;
}
