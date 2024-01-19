#pragma once

#include <iostream>
#include <tuple>
using namespace std;

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
