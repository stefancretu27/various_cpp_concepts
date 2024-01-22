#pragma once
#include <memory>
/*
* Insights on template class:
* 1. A template class that is implemented in a .cpp file requires another .cpp file where it is explicitly instantiated with the needed
* data types. As explained above, this need comes from the fact that the template is not compiled by itslef, instead it is replicated when
* an explicit instance is done. Furthermore, the additional .cpp file needs access to the template implementations of the class methods in
* order to properly generate the code for each explicit instantiation. Thus, the .cpp file with template implementations must be included
* in the additional .cpp file that holds the explicit instantiations.
 
* 2. A template class can be fully specialized. That is, a separate implementation is provided using explicit data types for all its* template parameters. In this case, the class declaration starts with template<>, keeping the template parameter list empty, whereas
* the explicit data types are provided after the class name (ClassName<dataType1, dataType2>) when the class is defined and when its methods
* are implemented in a separate file. In addition, when implementing its methods the template<> specifier is not needed anymore. 
* Since all its data type are explicitly specified, it is clear that a full specialization does not require explicit instantiation. Its
* syntax is mostly like the one of a plain class.

* 3. Unlike template functions, a template class can be [partially] specialized. That is, a separate implementation is provided for some
* of its template type parameters. Those parameters that are not specialized, remain general and the syntax for them stays similar with regular
* template classes. For the specialized types, the syntax follows the principles fo full specialization in the sense that the explicit types
* must be included after the class name when defining and and when implementing its methods.
* Like a template class beholding general types, a partially specialized class requires explicit intantiation with the needed types for its
* generic template parameters.
* Partial specialization can also be done for pointers or refs. Even though there is only one template parameter for which the specialization
* is implemented, it is still considered a partial specialization, as the type is not explicitly specified. In this case, the syntax is similar
* full specializations, only that after the class name is specified the pointer/ref type (ClassName<T*>) when defining the class and when
* implementing its methods.
*/

template<class S, class T>
class TemplateClass
{
    public:
    TemplateClass();
    explicit TemplateClass(const S& s, const T& t);
    TemplateClass(const TemplateClass& tc);
    TemplateClass(TemplateClass&& tc);
    TemplateClass& operator=(TemplateClass tc);
    void swap(TemplateClass& tc);
    //define friend swap function for being used by std algorithms
    friend void swap(TemplateClass<S, T>& lhs, TemplateClass<S, T>& rhs);
    
    const std::unique_ptr<S>& GetMUptrData() const {return mUptrData;};
    T GetMMember() const {return mMember;};
    
    private:
    std::unique_ptr<S> mUptrData;
    T mMember;
};
