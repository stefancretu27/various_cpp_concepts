/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>

#include "LanguageFeatures.hpp"
#include "InlineVariable.hpp"
using namespace std;


template<class T>
constexpr void ff(const T& val)
{
    cout<<__func__<<" "<<__LINE__<<" "<<val<<endl;
    //does not work with ref to rvalue arg
    if constexpr (is_integral<T>::value)
    {
        cout<<__LINE__<<" "<<val<<endl;
    }
}

struct DataForBinding
{
    std::string str;
    char c;
    std::pair<int, double> mPair;
    std::tuple<bool, int_least32_t, float> mTuple;
};

template<class T>
class PureVirtualTemplate
{
    public:
    virtual ~PureVirtualTemplate() = default;
    virtual T GetT() const = 0;
};

template<class T>
class VirtualTemplate : public PureVirtualTemplate<T>
{
    public:
    explicit VirtualTemplate(const T& t):member{t}{};
    virtual T GetT() const override {return member;};
    
    private:
    T member;
};

int main()
{
    std::cout<<"----------------------order of operations examples"<<std::endl;
    int i{};
    int res{27};
    
    cout<<++i<<" "<<i++<<endl;
    
    i = ++i + 2;
    cout<<i<<endl;
    
    std::cout<<"----------------------lambda capturing *this & this examples"<<std::endl;
    MyClass inst{};
    inst.Wrapper();
    cout<<inst.GetI()<<" "<<inst.GetStr()<<endl;
    inst.SecondWrapper();
    cout<<inst.GetI()<<" "<<inst.GetStr()<<endl;
    
    std::cout<<"----------------------if constexpr & lambda constexpr examples"<<std::endl;
    ff(res);
    FunctionWithConstexpr(res);
    std::function<void(const int)> func = [&i](const int ii){cout<<i+ii<<endl;};
    auto funcbind = std::bind(func, 17);
    FunctionWithConstexpr(funcbind);
    std::cout<<is_function<decltype(func)>::value<<" "<<is_function<decltype(funcbind)>::value<<" "<<is_function<std::function<void(const int)>>::value;
    cout<<" "<<is_function<void(const int)>::value<<endl;
    
    std::unique_ptr<int> upInt{make_unique<int>(8)};
    FunctionWithConstexpr(upInt.get());
    
    std::cout<<"----------------------inline variable&functions examples"<<std::endl;
    std::cout<<"before "<<inlineConstInt<<" "<<inlineDouble<<std::endl;
    InlineVarFunc();
    std::cout<<"after "<<inlineConstInt<<" "<<inlineDouble<<std::endl;
    InlineFunc();
    
    std::cout<<"----------------------structured bindings examples"<<std::endl;
    array<int, 5> arrayInt{5, 0};
    arrayInt[2] = 3;
    arrayInt[3] = 2;
    arrayInt[4] = 8;
    auto&& [val1, val2, val3, val4, val5] = arrayInt;
    std::cout<<val1<<" "<<val2<<" "<<val3<<" "<<val4<<" "<<val5<<std::endl;
    
    DataForBinding dfb{"data for binding", '!', {1, 3.14159}, {true, 47569, -2.308}};
    auto& [str, ch, pr, tpl] = dfb;
    std::cout<<str<<" "<<ch<<" "<<pr.first<<" "<<pr.second<<" "<<std::get<0>(tpl)<<" "<<std::get<1>(tpl)<<" "<<std::get<2>(tpl)<<std::endl;
    auto& [f, s] = dfb.mPair;
    std::cout<<f<<" "<<s<<std::endl;
    auto&& [g1, g2, g3] = tpl;
    std::cout<<g1<<" "<<g2<<" "<<g3<<std::endl;
    
    std::cout<<"----------------------initializers for if&switch examples"<<std::endl;
    std::map<int, std::string> mapIntStr;
    mapIntStr.insert({1, "str"});
    mapIntStr.emplace(2, "second string");
    mapIntStr.emplace_hint(mapIntStr.begin(), 0, "zero");
    
    if(auto it = mapIntStr.find(1); it!=mapIntStr.end())
    {
        std::cout<<it->first<<" "<<it->second<<std::endl;
    }
    
    switch(auto it = mapIntStr.find(1); it!=mapIntStr.end())
    {
        case false:
            cout<<"Not found in map"<<endl;
            break;
        case true:
            cout<<it->second<<endl;
    }
    
    std::cout<<"----------------------guaranteed copy ellision examples"<<std::endl;
    {
        std::cout<<"return T, an object constructed directly in the destination object => ellision guaranteed"<<std::endl;
        //bypassing temporary object creation in return statement, then copying it in the copy initialized value fTRet
        T fTRet = funcT();
    }
    {
        std::cout<<"return T, internally created, an object constructed directly in the destination object => ellision guaranteed"<<std::endl;
        //bypassing temporary object creation in return statement, then copying it in the copy initialized value fTRet
        T fTRet2 = func2T();
    }
    {
        std::cout<<"return U, derived from T, as T inst =>copy/move not ellided"<<endl;
        T fURet = funcU();
    }
    {
        std::cout<<"return U, internally created, derived from T, as T inst =>copy/move not ellided"<<endl;
        T fU2Ret = func2U();
    }
    {
        std::cout<<"uniform initialize T from temporary object T() => ellision guaranteed"<<endl;
        T instT{T()};
    }
    {
        std::cout<<"uniform initialize U from temporary object U() => ellision guaranteed"<<endl;
        U instU{U()};
    }
    {
        std::cout<<"uniform initialize T&& from U()"<<endl;
        T&& refinstT{U()};
    }
    {
        std::cout<<"uniform initialize T from U() => copy/move not ellided"<<endl;
        T tInst{ U{}};
    }

    std::cout<<"----------------------temporary materialization examples"<<std::endl;
    std::cout<<TempMater(5).i<<std::endl;
    
    std::cout<<"----------------------fold expressions examples"<<std::endl;
    PrintArgs(1, "string", 'c', 3.14159);
    
    std::cout<<SumArgs(3.3, -6.5, 19.09, 22.1, -33.55)<<" "<<ProdArgs(3, 4, 1, 2)<<" "<<AndArgs(0,2,3,-12)<<" "<<OrArgs(0,2,3,-12)<<" "<<DiffArgs(5,3,2,1,0)<<endl;
    
    VecPushBack(std::string{}, std::string("1"), std::string("str"), std::string("doi"), std::string("4th"), std::string("another"));
    LambdaCall("tac", 2, "pac", 'g', -2.7182);
    
    VirtualTemplate<int> vt{2};
    cout<<vt.GetT()<<endl;
    std::cout<<"----------------------template argument deduction examples"<<std::endl;
    AggregateStruct agg1{1, 'c'};
    AggregateStruct agg2 = {2, 'd'};
    
    TemplateClass tempClass1{1,2,3};
    cout<<tempClass1.GetMember()<<endl;
    

    return 0;
}
