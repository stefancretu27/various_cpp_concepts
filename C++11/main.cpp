#include <iostream>
#include <forward_list>

#include "CopyAndSwapIdiom.hpp"
#include "LanguageFeatures.hpp"

using namespace std;

template<class T>
void RefParam(const T& refToLvalue)
{
    cout<<"refToLvalue"<<endl;
    cout<<refToLvalue<<endl;
}

template<class T>
void RefParam(T&& refToRvalue)
{
    cout<<"refToRvalue"<<endl;
    cout<<refToRvalue<<endl;
}

template<class T>
struct Wrapper
{
    static void RefParam(const T& refToLvalue)
    {
        cout<<"refToLvalue"<<endl;
        cout<<refToLvalue<<endl;
    }

    static void RefParam(T&& refToRvalue)
    {
        cout<<"refToRvalue"<<endl;
        cout<<refToRvalue<<endl;
    }
};


int main()
{
    forward_list<char> flc{'a', 'b', 'v', 'x'};
    CheckType(flc);
    
    std::pair<int, string> intStrPair{make_pair(1, "first")};
    auto key = intStrPair.first;
    decltype(key) variableWithTypeOfKey{};
    cout<<"variableWithTypeOfKey "<<++variableWithTypeOfKey<<endl;
    
    Composed comp{{1,2,3,4,5}};
    Composed& lref{comp};
    //Composed&& rref{comp};      //error: cannot bind rvalue reference of type ‘Composed&&’ to lvalue of type ‘Composed’
    //Composed&& rreflref{lref};   //same as above
    Composed&& rref{std::move(comp)};       //std::move on l-value acts as static_cast to a r-value reference
    Composed&& rreflref{std::move(lref)};
    Composed another = std::move(comp);     //call std::move to obtain ref to r-value, forcing the call of move c-tor
    
    
    cout<<"call RefParam on Composed"<<endl;
    Composed anotherComp{{-1,-2,-3,-4,-5}};
    RefParam<Composed>(anotherComp);                //calls the l-value overload
    RefParam(anotherComp);                          //calls r-value overload due to type deduction which does not deduce const cv-qualification from the argument =>
                                                    //it resolves to Composed&, not to const Composed&
    Wrapper<Composed>::RefParam(anotherComp);       //calls the l-value overload
    
    cout<<"call RefParam on const Composed"<<endl;
    const Composed constComp{{-1,-2,-3,-4,-5}};
    //RefParam<Composed>(constComp);                  //calls the l-value overload
    RefParam(constComp);                            //calls l-value overload due to type deduction which is able to deduce const cv-qualification from the argument =>
                                                    //it resolves to const Composed&
    Wrapper<Composed>::RefParam(constComp);         //calls the l-value overload
    
    cout<<"call RefParam on Composed&"<<endl;
    Composed& anotherlref{anotherComp};
    RefParam<Composed>(anotherlref);                //calls the l-value overload
    RefParam(anotherlref);                          //calls r-value overload due to type deduction. If it were a ref to const, it would solve to ref to const l-value
    Wrapper<Composed>::RefParam(anotherlref);       //calls the l-value overload
    
    cout<<"call RefParam on const Composed&"<<endl;
    const Composed& constlref{anotherComp};
    //RefParam<Composed>(constlref);                  //calls the l-value overload
    RefParam(constlref);                            //calls l-value overload due to type deduction which can deduce const cv-qualifier from the argument's type
    Wrapper<Composed>::RefParam(constlref);         //calls the l-value overload 
    
    cout<<"call RefParam on Composed&&"<<endl;
    Composed&& anotherrref{std::move(anotherComp)};
    RefParam<Composed>(anotherrref);                            //the l-value overload is called, as the reference becomes a l-value itself
    RefParam(anotherrref);                                      //calls r-value overload due to type deduction => type is deduced as Composed&
    RefParam<Composed>(std::forward<Composed>(anotherrref));    //calls r-value overload as the argument's type is perfectly forwarded, preserving cv-qualification 
                                                                //and ref type
                                                                
    cout<<"call RefParam on const Composed&&"<<endl;
    const Composed&& constrref{std::move(anotherComp)};
    RefParam<Composed>(constrref);                            //the l-value overload is called, as the reference becomes a l-value itself + const deduction
    RefParam(constrref);                                      //calls l-value overload due to type deduction => type is deduced as const Composed&
    RefParam<Composed>(std::forward<const Composed>(constrref)); //calls l-value overload as the argument's type is perfectly forwarded, preserving cv-qualification, 
                                                                // as it is ref to const, and ref type
    UnscopedColor uColor{ORANGE};
    cout<<"UnscopedColor type initialized from enumerator defaults to int implicitly "<<uColor<<endl;
    ScopedColor sColor{ScopedColor::GREEN};
    cout<<"ScopedColor type initialized from scoped enumerator does not defaults to int implicitly "<<static_cast<unsigned short>(sColor)<<endl;
    
    std::string str{"str"};
    ExampleConstexprMethod ecm{};
    const auto res = ecm.ConstexprFunc(1, 'c', str, constrref);         //cannot be constexpr as value is computed at runtime
    const auto res2 = ecm.ConstexprFunc(-11, 'g', str, constrref);
    
    return 0;
}
