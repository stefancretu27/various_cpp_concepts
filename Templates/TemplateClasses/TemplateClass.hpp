#pragma once
#include <memory>

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
