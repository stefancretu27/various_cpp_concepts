#include "TemplateClass.hpp"

template<class S, class T>
TemplateClass<S, T>::TemplateClass():mUptrData{std::make_unique<S>()}
{
    
}

template<class S, class T>
TemplateClass<S, T>::TemplateClass(const S& s, const T& t):mUptrData{std::make_unique<S>(s)}, mMember{t}
{
    
}

template<class S, class T>
TemplateClass<S, T>::TemplateClass(const TemplateClass<S, T>& tc)
{
    //unique pointers point to same value, but they are different instances, pointing to different dinamically allocated memory addresses
    mUptrData = std::make_unique<S>(*tc.mUptrData);
    mMember = tc.mMember;
}

template<class S, class T>
TemplateClass<S, T>::TemplateClass(TemplateClass<S, T>&& tc)
{
    //unique pointers point to same value, but they are different instances, pointing to different dinamically allocated memory addresses
    mUptrData = std::move(tc.mUptrData);
    mMember = std::move(tc.mMember);
}

template<class S, class T>
void TemplateClass<S, T>::swap(TemplateClass<S, T>& tc)
{
    mUptrData.swap(tc.mUptrData);
    std::swap(mMember, tc.mMember);
}

template<class S, class T>
TemplateClass<S, T>& TemplateClass<S, T>::operator=(TemplateClass<S, T> tc)
{
    tc.swap(*this);
    return *this;
}

template<class S, class T>
void swap(TemplateClass<S, T>& lhs, TemplateClass<S, T>& rhs)
{
    //use internally defined swap
    lhs.swap(rhs);
}

//explicit instantiation
template class TemplateClass<int, double>;
