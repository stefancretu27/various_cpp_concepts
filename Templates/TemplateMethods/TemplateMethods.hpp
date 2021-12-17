#pragma once

#include <iostream>
#include <stdint.h>


class NonTemplateClass
{
    private:
    uint_least16_t mUInt16;

    template<class T, class S>
    struct EncapsulateTemplateMethod
    {
        static void wrappedDoWork(const NonTemplateClass& obj, const T& t, const S& s);
    };

    public:
    NonTemplateClass() = default;
    NonTemplateClass(uint_least16_t u);
    uint_least16_t getData() const {return mUInt16;};

    template <class T>
    void doWork(const T input);
    //overload for bool
    void doWork(const bool t);
    //explicit specialization is not allowed within class as it required for the class to be specialized
    // template<>
    // void doWork<char>(const char input);

    //the below method cannot be fully/partially specialized by itself, only that it wraps the call for the static method of the struct above which
    //can be fully/partially specialized. However, the method below can still be overloaded
    template <class T, class S>
    void doWork(const T& t, const S& s);
};