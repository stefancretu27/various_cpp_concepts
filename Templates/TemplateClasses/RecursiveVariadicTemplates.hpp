#pragma once

#include <iostream>
#include <tuple>
using namespace std;

//variadic template struct
template<class T, class ... Args>
struct VariadicArgsExample
{
    static T computeSum(T firstValue, Args... args)
    {
        if(is_fundamental<T>::value)
        {
            return firstValue + VariadicArgsExample<Args...>::computeSum(args...);
        }
    }
};

//partial specialization with 1 argument for the struct above 
template<class T>
struct VariadicArgsExample<T>
{
    static T computeSum(T singleValue)
    {
        return singleValue;
    }
};


//example of a printf using variadic templates and cout
void myPrintf(const char* format)
{
    cout<<*format;
}

template<class T, class ... Args>
void myPrintf(const char* format, T firstValue, Args ... args)
{
    for(; *format!='\0'; ++format)
    {
        //if a format specifier is encountered, the actual type specifier is omitted
        //as cout knows how to interpret them
        if(*format == '%')
        {
            //cout the input value and recursively call with the format poinitng
            //to the next character and with expanded pack
            cout<<firstValue;
            myPrintf(format + 1, args...);
            break;
        }
        else
        {
            cout<<*format;
        }
    }
}

//------------------------------------------------------------------------------

//base template struct
template<size_t argsCount, class ... ArgsTypes>
struct ArgsTuple
{
    static void PrintArgs(ArgsTypes ... args)
    {
        //convert variadic template args into tuple
        auto tupleArgs = make_tuple<ArgsTypes...>(std::move(args)...); //make_tuple requires r-value ref args => must use move
        const size_t tupleSize{tuple_size<decltype(tupleArgs)>::value};
        
        cout<<endl;
        
        ArgsTuple<tupleSize, decltype(tupleArgs)>::PrintArgs(tupleArgs);
        
        cout<<endl;
    }
};

//partially specialize the above base template for when there is a tuple instead of variadic template args
template<size_t argsCount, class TupleType>
struct ArgsTuple<argsCount, TupleType>
{
    static void PrintArgs(TupleType tuple)
    {
        ArgsTuple<argsCount-1, decltype(tuple)>::PrintArgs(tuple);
        std::cout<<std::get<argsCount-1>(tuple)<<" ";
    }   
};

//partially specialize the above specialization for 1-element tuple. The TupleType is still a template type, the specialization is of the argsCount
template<class TupleType>
struct ArgsTuple<1, TupleType>
{
    static void PrintArgs(TupleType tuple)
    {
        std::cout<<std::get<0>(tuple)<<" ";
    }
};


//------------------------------------------------------------------------------

//base template struct
template<class ... ArgsTypes>
struct DataPrinter
{
};

//partially specialized for one arg type
template<class ArgType>
struct DataPrinter<ArgType>
{
    public:
    static void PrintData(ArgType arg)
    {
        std::cout<<arg<<std::endl;
    };
};

//partially specialized for variadic template args + first arg type
template<class FirstArgType, class ... ArgsTypes>
struct DataPrinter<FirstArgType, ArgsTypes...>
{
    public:
    static void PrintData(FirstArgType firstArg, ArgsTypes ... remainingArgs)
    {
        std::cout<<firstArg<<" ";
        DataPrinter<ArgsTypes...>::PrintData(remainingArgs...);
    };
};
