#include "TemplateMethods.hpp"

#include <type_traits>

//implement template method that has only 1 template parameter type
template <class T>
void NonTemplateClass::doWork(const T input)
{
    if(std::is_integral<T>::value)
    {
        std::cout<<mUInt16*input<<std::endl;
    }
    else
    {
        std::cout<<"template type parameter is not integral type"<<std::endl;
    }
}

//implement template method that has 2 template parameter types
//the actual template method that is accessible to class users wraps the method that is subject to specialization
template<class T, class S>
void NonTemplateClass::doWork(const T& t, const S& s)
{
    EncapsulateTemplateMethod<T,S>::wrappedDoWork(*this, t, s);
}

// Implementation of the wrapped method for general cases. Here, it is provided in the struct encapsulated by the non template class
template <class T, class S>
void NonTemplateClass::EncapsulateTemplateMethod<T,S>::wrappedDoWork(const NonTemplateClass& obj, const T& t, const S& s)
{
    static_assert(std::is_floating_point<T>::value, "template type parameter is not floating point type");
    std::cout<<obj.mUInt16*t<<std::endl;

    if(std::is_floating_point<S>::value)
    {
        std::cout<<obj.mUInt16*t*s<<std::endl;
    }
    else
    {
        try
        {
            std::cout<<obj.mUInt16*t*static_cast<int_least16_t>(s)<<std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

// Partial specialization of the template struct with T=bool. Such declaration could be included in header, to make it
// obvious which specializations do exist. However, if it is desired to hide such implementation details, the approach
// used in this file is accepted.
template<class S>
struct NonTemplateClass::EncapsulateTemplateMethod<bool, S>
{
    static void wrappedDoWork(const NonTemplateClass& obj, bool t, const S& s);
};

// the implementation of the partial specialization is provided outside the class, for syntax proof sake
template<class S>
void NonTemplateClass::EncapsulateTemplateMethod<bool, S>::wrappedDoWork(const NonTemplateClass& obj, bool t, const S& s)
{
    if(std::is_floating_point<S>::value)
    {
        std::cout<<obj.mUInt16*s<<" "<<t<<std::endl;
    }
}

// Partial specialization of the template struct with S=bool
template<class T>
struct NonTemplateClass::EncapsulateTemplateMethod<T, bool>
{
    static void wrappedDoWork(const NonTemplateClass& obj, const T& t, bool s);
};

template<class T>
void NonTemplateClass::EncapsulateTemplateMethod<T, bool>::wrappedDoWork(const NonTemplateClass& obj, const T& t, bool s)
{
    if(std::is_floating_point<T>::value)
    {
        std::cout<<obj.mUInt16*t<<" "<<s<<std::endl;
    }
}

// Full specialization of the template struct with S=bool and T=bool
template<>
struct NonTemplateClass::EncapsulateTemplateMethod<bool, bool>
{
    static void wrappedDoWork(const NonTemplateClass& obj, bool t, bool s)
    {
        std::cout<<obj.getData()<<" "<<t<<" "<<s<<std::endl;
    }
};

