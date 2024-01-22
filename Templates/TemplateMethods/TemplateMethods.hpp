#pragma once

#include <iostream>
#include <stdint.h>

/* Insights on template functions and methods:
* 1. The template functions represent a pattern, that uses placeholder for generic types, which is used to create similar functions, with 
* specific data types. Thus, the same piece of code is reused in distinct contextes with distinct data types, that should fit in the 
* function's logic (eg: does not make sense to pass char or string data types to a template function performing some numeric operations).

* 2. A template function's definition includes a template parameter declaration which specifies that some placeholders are to be used
* for the data types that are supposed to be generic. This generic declaration is not compiled directly, as the compiler does not know how to
* compile generic template parameter types. Instead, each time it is invoked, the data types for the generic types are explicitly specified,
* thus the compiler replicates the generic pattern and creates a function with the actual data types.
* In other terms, the compiler stencils out a copy of the generic definition, replaces the template type parameters
* with those provided by that call, creating a specific function, which is referred to as template function instance.

* 3. Like a plain function/method, a template function or method can be overloaded following the same principles. Nevertheless, a template
* specialization does not overload.

* 4. A template method can be defined inside a non template class. In this case, it is recommended that the implementation of the template 
* methods should be done in a separate .cpp file from the .cpp file where the non-template methods are implemented. Thus, the said source file
* can be included in the .cpp file where the templates are intasntiated with the used data types, such that the compiler can resolve the calls

* 5. Template functions cannot be partially specialized, but only fully specialized. Nevertheless, in such cases, as the data types are known
* it is preferred to implement an overload. That is due to the steps of resolving a function's call:
*     i) Firstly, non template implementations are looked for. The one that best matches the parameter types is chosen, if any.
*     ii) Secondly, the templates are looked for. The best overload is chosen, if any.
*     iii) Lastly, for the most suitable overload, it is looked for a specialization, if any, that best matches the call
* As it can be seen, providing a specialization might lead to situation when it is ignored when resolving a call, if there is an overload
* that is considered to be a good fit, because it depends for which overload that specialization was implemented.
* On top of that template method overloading should be preferred to explicit specialization for one more reason: template methods caanot
* be fully specialized within a class, unless the class is specialzied itself.

* 6. The full specialization of a function/method is also called explicit specialization. When fully specializing a function/method,
* all the template parameters are omitted from the template declaration (thus, only template<> remains). The specialization entails that
* all data types corresponding to template types are known, therefore they can be directly used when implementing the full specialization
* Optionally, the data types can be inserted after the function's name, between <>, marking its name as templatized (void f<int, float>(args))
* Moreover, the specialization must have the same signature with the template definition it specializes, following similar principles as overriding
	
* 7. When writing a template function, if it is envisaged for specialization, besides overloading, it is recommended to encapsulate it, as a
* static method of astruct/class which can be full or partially specialized. Thereafter, the function shall be called via a wrapper whose 
* signature stays always the same. This wrapper shall not be subject of specialization.
* Another interesting aspect here is that when the struct/class encapsulating the static template method is fully or partially specialized
* the method's signature can be slightly changed (arguments types, return type, but not args number) and the specialization stays valid. 
* When a function is specialized, its signature should stay the same (return type, argument's types and const qualifiers, its constness)
	
* 8. The approach described above can also be applied when specializing template method of a non template class. Nonetheless, the syntax
* requires some adjustments. Firstly, the template struct which wraps the template method should eb defined inside the non template class
* for an easier access to members. Thereafter, for the wrapped method to be able to access members of the outer class, it has to receive as
* argument the instance (this pointer) of the calling instance of the non template class. The wrapped method stays as static and does not
* have a this pointer from the wrapping template struct.
*/

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
