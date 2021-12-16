#include "TemplateMethods.hpp"

#include <type_traits>

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

template<class T, class S>
void NonTemplateClass::doWork(const T& t, const S& s)
{
    EncapsulateTemplateMethod<T,S>::wrappedDoWork(*this, t, s);
}

//implementation of the wrapped method
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

//partial specialization of the template struct
template<class S>
struct NonTemplateClass::EncapsulateTemplateMethod<bool, S>
{
    static void wrappedDoWork(const NonTemplateClass& obj, bool t, const S& s);
};

template<class S>
void NonTemplateClass::EncapsulateTemplateMethod<bool, S>::wrappedDoWork(const NonTemplateClass& obj, bool t, const S& s)
{
    if(std::is_floating_point<S>::value)
    {
        std::cout<<obj.mUInt16*s<<" "<<t<<std::endl;
    }
}

//partial specialization of the template struct
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

//specialization of the template struct
template<>
struct NonTemplateClass::EncapsulateTemplateMethod<bool, bool>
{
    static void wrappedDoWork(const NonTemplateClass& obj, bool t, bool s)
    {
        std::cout<<obj.getData()<<" "<<t<<" "<<s<<std::endl;
    }
};

