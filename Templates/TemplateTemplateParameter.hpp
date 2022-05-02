#pragma once

#include <iostream>

/*
* Template struct overloading the function call () operator.
* A template function cannot be a template template argument. Instead, it can be encapsulated 
* in a class that can be a template template argument.
*/
template<class T>
struct Functor
{
    void operator()(T param)
    {
        if(is_fundamental<T>::value)
        {
            std::cout<<"value of the parameter: "<<param<<std::endl;
        }
    }
};

/*
* Encapsulate static method in a struct such that: 
*   - it is invokable without an instance, it can be 
*   - fully and partially specialized 
*   - can be a template template argument thorugh the struct
*/
template<class T>
struct FunctionWrapper
{
    static T doComputation(T arg1)
    {
        T result{arg1};
        
        if(is_fundamental<T>::value)
        {
            result = arg1 + arg1;
        }
        
        return result;
    }
};

/*
* This shows a way of implementing a callback mechanism: a struct that encapsulates
* a method envisaged for being called back, is used as template argument for a template
* function or class.
*/

// Template function that has 2 template template parameters whose template type parameter
// reuse T, the template parameter of the function (i.e. the same template type is shared amongst them)
template<class T, template<typename> class Funct, template<typename> class funcWrapper>
void function(T val)
{
    if(is_class<Funct<T>>::value)
    {
        Funct<T>()(val);
    }
    
    if(is_class<funcWrapper<T>>::value)
    {
        //if(is_function<decltype(funcWrapper<T>::doComputation)>::value)
        if(is_member_function_pointer<decltype(&funcWrapper<T>::doComputation)>::value)
        {
            std::cout<<funcWrapper<T>::doComputation(val)<<std::endl;
        }
    }
}

//template class that has a template template parameter another class whose instance is a member of the former. 
template<class T, template<typename> class Funct>
class MyClass
{
    public:
    MyClass() = default;
    ~MyClass() = default;
    
    void operator()(T param, T param2, T param3)
    {
        functorInst(param+param2);
	      //functor direct call, bypassing instance usage
	      Funct<T>()(param3);
    }
    
    private:
    Funct<T> functorInst;
};
