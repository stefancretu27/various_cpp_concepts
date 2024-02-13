#include <iostream>
#include <forward_list>
#include <initializer_list>

#include "CopyAndSwapIdiom.hpp"
#include "LanguageFeatures.hpp"
#include "LibraryFeatures.hpp"

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

Composed compo()
{
    cout<<__func__<<endl;
    return Composed{};
}

template<class T, typename enable_if<is_integral<T>::value, bool>::type=true>
bool IsOdd(const T elem)
{
    return (elem%2!=0);
}

template<class T>
struct IsPositive
{
    bool operator()(const T elem)
    {
        return elem > 0;
    }
};

template<class T, typename = enable_if_t<is_integral<T>::value||is_floating_point<T>::value, bool>>
struct CheckPosNeg
{
    bool IsNegative(const T value){return value < 0;};
    static bool IsPositive(const T value){return value > 0;};
};

template<class Iter>
void merge_sort(Iter first, Iter last)
{
    if (last - first > 1)
    {
        Iter middle = first + (last - first) / 2;
        merge_sort(first, middle);
        merge_sort(middle, last);
        std::inplace_merge(first, middle, last);
    }
}

int main()
{
    cout<<"------------------------------decltype----------------------------------------"<<endl;
    forward_list<char> flc{'a', 'b', 'v', 'x'};
    CheckType(flc);
    
    std::pair<int, string> intStrPair{make_pair(1, "first")};
    auto key = intStrPair.first;
    decltype(key) variableWithTypeOfKey{};
    cout<<"variableWithTypeOfKey "<<++variableWithTypeOfKey<<endl;
    
    cout<<"------------------------------ref to rvalues--------------------------------"<<endl;
    Composed comp{{1,2,3,4,5}};
    Composed& lref{comp};
    //Composed&& rref{comp};      //error: cannot bind rvalue reference of type ‘Composed&&’ to lvalue of type ‘Composed’
    //Composed&& rreflref{lref};   //same as above
    Composed&& rref{std::move(comp)};       //std::move on l-value acts as static_cast to a r-value reference
    Composed&& rreflref{std::move(lref)};
    Composed another = std::move(comp);     //call std::move to obtain ref to r-value, forcing the call of move c-tor
    
    cout<<"------------------------------template type deduction-----------------------"<<endl;
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
    
    cout<<"------------------------------scoped enums--------------------------------------"<<endl;
    UnscopedColor uColor{ORANGE};
    cout<<"UnscopedColor type initialized from enumerator defaults to int implicitly "<<uColor<<endl;
    ScopedColor sColor{ScopedColor::GREEN};
    cout<<"ScopedColor type initialized from scoped enumerator does not defaults to int implicitly "<<static_cast<unsigned short>(sColor)<<endl;
    
    cout<<"------------------------------constexpr----------------------------------------"<<endl;
    std::string str{"str"};
    ExampleConstexprMethod ecm{};
    const auto res = ecm.ConstexprFunc(1, 'c', str, constrref);         //cannot be constexpr as value is computed at runtime
    const auto res2 = ecm.ConstexprFunc(-11, 'g', str, constrref);
    
    std::string initListStr{'a', 'b', 'c', 'm'};
    cout<<initListStr<<endl;
    
    int d1, d2(), d3{};
    cout<<d1<<" "<<d2<<" "<<d3<<endl;
    
    Composed compo;
    compo.PrintContainer();
    
    cout<<"------------------------------initializer_list<T>----------------------------------------"<<endl;
    initializer_list<int> initList{1,2,3,4,5,6,7,8,9};
    for(auto it = initList.begin(); it != initList.end(); ++it)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
    
    WithInitList wil{7,8,9,0};
    wil.PrintList();
    
    cout<<"------------------------------delegate and inherited c-tors-------------------"<<endl;
    
    DelegateCtor delegate1{1, 2.12};
    UsingCtor using1(23,-5.6);
    UsingCtor using2{};
    UsingCtor using3{'f'};
    
    cout<<"------------------------------union----------------------------------------"<<endl;
    
    UnionExample uEx;
    uEx.c = 'c';
    uEx.n = 100;
    cout<<uEx.c<<" "<<uEx.n<<" "<<sizeof(uEx)<<endl;
    uEx.s[0] = 0x00AB12;
    uEx.s[1] = 0x00ef3c;
    cout<<hex<<uEx.s[0]<<" "<<uEx.s[1]<<endl;
    cout<<uEx.s[0]<<" "<<uEx.s[1]<<endl;
    
    ClassesMembersUnion cmu{"must be initialized so it knows which member is active"};  
    cmu.str.~basic_string();                        //must be explicitly destroyed
    new (&cmu.vec) std::vector<int>();              //must be explicitly allocated
    cmu.vec.emplace_back(55);
    cmu.vec.~vector<int>();                           //must be explicitly destroyed

    cout<<"------------------------------align----------------------------------------"<<endl;
    
    AlignExample ae1{};
    cout<<"alignment: "<<alignof(AlignExample)<<" sizeof: "<<sizeof(AlignExample)<<endl;

    forward_list<int> lInt{1,2,3,4,5,5,4,3,3,3,2,1};
    
    std::cout<<"-------search operations: all_of, any_of, none_of, find, find_if, find_if_not, count, count_if, count_if_not--------"<<endl;
    bool areAllPositive = all_of(lInt.begin(), lInt.end(), IsPositive<int>{});
    bool isAnyNegative = any_of(lInt.begin(), lInt.end(), [](const int elem){return elem < 0;});
    bool isNoElemOdd = none_of(lInt.begin(), lInt.end(), IsOdd<int>);
    cout<<"areAllPositive: "<<areAllPositive<<" isAnyNegative: "<<isAnyNegative<<" isNoElemOdd "<<isNoElemOdd<<endl;
    
    auto itElemEqualTo5 = find(lInt.begin(), lInt.end(), 5);
    auto itElemGT3 = find_if(lInt.begin(), lInt.end(), [](const int elem){return elem > 3;});
    //searches for an element for which the condition is not satisfied aka returns false
    auto itElemNotNeg = find_if_not(lInt.begin(), lInt.end(), [](const int elem){return elem > 0;});
    cout<<"itElemEqualTo5: "<<*itElemEqualTo5<<" itElemGT3: "<<*itElemGT3<<" itElemNotNeg "<<((itElemNotNeg!=lInt.end())?to_string(*itElemNotNeg):"not exist")<<endl;
    
    auto noElemsEqual4 = count(lInt.begin(), lInt.end(), 4);
    auto noElemGT2LT5 = count_if(lInt.begin(), lInt.end(), [](const int elem){ return elem>2&&elem<5;});
    cout<<"noElemsEqual4 "<<noElemsEqual4<<" noElemGT2LT5 "<<noElemGT2LT5<<endl;
    
    array<string, 14> arrayStr{"zero", "0", "1st", "first", "second", "2nd", "3rd", "fourth", "4th", "second", "2nd", "2", "fifth", "5th"};
    vector<string> vecStr{"second", "2nd"};
    auto searchIt = search(arrayStr.begin(), arrayStr.end(), vecStr.begin(), vecStr.end());
    auto findendIt = find_end(arrayStr.begin(), arrayStr.end(), vecStr.begin(), vecStr.end());
    cout<<"searchIt "<<distance(arrayStr.begin(), searchIt)<<" find_end "<<distance(arrayStr.begin(),findendIt)<<endl;
    
    std::cout<<endl<<"-------copy operations: copy, copy_if, copy_n, move, move_backward--------"<<endl;
    list<int> listInt{};
    listInt.resize(6);
    array<int, 6> arrayInt{11,22,33,44,55,77};
    auto copyIt = copy(arrayInt.begin(), arrayInt.end(), listInt.begin());
    cout<<"list elements copy from array list size: "<<listInt.size()<<" retIt equals list.end(): "<<(copyIt==listInt.end())<<endl;
    for(auto rit = listInt.rbegin(), rend = listInt.rend(); rit!=rend; ++rit)
    {
        cout<<*rit<<" ";
    }
    cout<<endl;
    
    cout<<"copy_n 3 elements from list to array+2 + iterator value "<<endl;
    auto copyNIt = copy_n(lInt.begin(), 3, arrayInt.begin()+2);
    for(auto it = arrayInt.begin(), end = arrayInt.end(); it!=end; ++it)
    {
        cout<<*it<<" ";
    }
    cout<<" ret iterator: "<<*copyNIt<<" equals arrayInt.end(): "<<(copyNIt==arrayInt.end())<<endl;
    
    cout<<"copy_if elem<50 elements from list<int> to fwd_list<int>: "<<endl;
    forward_list<int> fwdList(6);
    auto copyIfIt = copy_if(listInt.begin(), listInt.end(), fwdList.begin(), [](const int elem){return elem<50;});
    for(auto&& elem : fwdList)
    {
        cout<<elem<<" ";
    }
    cout<<" ret iterator: "<<((copyIfIt!=fwdList.end())?*copyIfIt:(int)(copyIfIt==fwdList.end()))<<endl;
    
    cout<<"move string elements from fwd_list to vector(4) =>fwd_list has size 4 but no elements: "<<endl;
    vector<string> vec4Str(4);
    forward_list<string> fwdListStr{"elem", "item", "example object", "thing"};
    auto moveIt = move(fwdListStr.begin(), fwdListStr.end(), vec4Str.begin());
    cout<<" vec.size(): "<<vec4Str.size()<<" fwd_list size can be obtained with distance(), as it has no size() "<<distance(fwdListStr.begin(), fwdListStr.end())<<endl;
    for(auto&& elem : fwdListStr)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    for(auto&& elem : vec4Str)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    
    cout<<"move_backward to deque<int> from subarray, then find_end deque (subarray) in array<int, 6>: "<<endl;
    deque<int> deqInt(3);
    auto moveBackIt = move_backward(next(arrayInt.begin(), 1), prev(arrayInt.end(), 2), deqInt.end());
    for(auto&& elem : deqInt)
    {
        cout<<elem<<" ";
    }
    auto findEndIt = find_end(arrayInt.begin(), arrayInt.end(), deqInt.begin(), deqInt.end());
    cout<<" ret iterator: "<<*findEndIt<<" distance from array.begin()"<<distance(arrayInt.begin(), findEndIt)<<endl;
    
    cout<<"move_backward from deque<string> to list<string>, with deque being emptied as strings are moved "<<endl;
    deque<string> deqStr{"first", "second", "3rd", "4th", "fifth"};
    list<string> listStr(5);
    auto moveStrIt = move_backward(deqStr.begin(), deqStr.end(), listStr.end());
    cout<<" deque size "<<distance(begin(deqStr), end(deqStr))<<" list size "<<listStr.size()<<endl;
    for(auto&& str : deqStr)
    {
        cout<<str<<" ";
    }
    cout<<endl;
    for(auto&& str : listStr)
    {
        cout<<str<<" ";
    }
    cout<<endl;
    
    std::cout<<endl<<"-------copy operations: swap, iter_swap, swap_ranges--------"<<endl;
    
    list<string> list3Str{"str1", "str2", "str3"};
    cout<<"iter_swap list3.begin()+2  with lis5.begin()+3"<<endl;
    iter_swap(next(list3Str.begin(), 2), next(listStr.begin(), 3));
    for(auto&& str : listStr)
    {
        cout<<str<<" ";
    }
    cout<<endl;
    for(auto&& str : list3Str)
    {
        cout<<str<<" ";
    }
    cout<<endl;
    
    cout<<"swap_ranges with [list5.begin()+1, +3) with list3"<<endl;
    swap_ranges(next(listStr.begin(), 1), next(listStr.begin(), 3), list3Str.begin());
    for(auto&& str : listStr)
    {
        cout<<str<<" ";
    }
    cout<<endl;
    for(auto&& str : list3Str)
    {
        cout<<str<<" ";
    }
    cout<<endl;
    
    std::cout<<endl<<"-------transformation operations: transform, replace, replace_if--------"<<endl;
    vector<string> resVecStr(listStr.size()-2);
    cout<<"list of strings, but first and last elements, get extension (concat) via transform, result is stored in vector of strings"<<endl;
    auto transformIt = transform(next(listStr.begin()), prev(listStr.end()), resVecStr.begin(), [](string& str){ return str+="_transformed";});
    for(auto&& str : resVecStr)
    {
        cout<<str<<" ";
    }
    cout<<endl;
    
    cout<<"replace value 77 with 88 in arrayInt"<<endl;
    replace(arrayInt.begin(), arrayInt.end(), 77, 88);
    for(auto&& elem : arrayInt)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    cout<<"replace_if val<30&&val>10 with 200 in arrayInt"<<endl;
    replace_if(arrayInt.begin(), arrayInt.end(), [](const int val){return val<30&&val>10;}, 200);
    for(auto&& elem : arrayInt)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    
    std::cout<<endl<<"-------generation operations: fill, fill_n, generate, generate_n--------"<<endl;
    list<int> listInts(7);
    fill(listInts.begin(), listInts.end(), -12);
    generate_n(listInts.begin(), 4, [](){static int k{}; return (++k%2)?2:1;});
    cout<<"fwd_list<int>(7) elements firstly fill with -12, then apply 2 on even index and 1 on odd index, to he first 4 elems"<<endl;
    for(auto&& elem : listInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    
    std::cout<<endl<<"-------removing operations: remove, remove_if, unique, unique_copy, remove_copy, remove_copy_if--------"<<endl;
    auto removeIt = remove(listInts.begin(), listInts.end(), 2);
    cout<<"fwd_list<int>(7) elements after remove values=2 =>container is not resized, garbage values fill part between the logical end and the physical end"<<endl;
    for(auto&& elem : listInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    
    removeIt = remove_if(listInts.begin(), listInts.end(), [](const int val){return val<=0;});
    cout<<"erase-remove idiom: erase garbage elements after remove is applied to container, using the iterator returned by remove:"<<endl;
    listInts.erase(removeIt, listInts.end());
    for(auto&& elem : listInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    
    cout<<"generate int values:"<<endl;
    forward_list<int> fwdListInts(9);
    generate(fwdListInts.begin(), fwdListInts.end(), [](){static int k{}; return (++k%2)?k:-k;});
    for(auto&& elem : fwdListInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    
    cout<<"new container values after remove_copy_if val>=0 from the above:"<<endl;
    listInts.resize(4);
    removeIt = remove_copy_if(fwdListInts.begin(), fwdListInts.end(), listInts.begin(), [](const int val){return val>=0;});
    for(auto&& elem : listInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    
    std::cout<<endl<<"-------order changing operations: reverse, reverse_copy, rotate, rotate_copy--------"<<endl;
    auto pivot = next(fwdListInts.begin(), 6);
    cout<<" rotate pivot value before rotate: "<<*pivot<<endl;
    auto rotateIt = rotate(fwdListInts.begin(), pivot, fwdListInts.end());
    cout<<" rotate pivot value after rotate: "<<*pivot<<" ret iterator: "<<*rotateIt<<endl;
    for(auto&& elem : fwdListInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    
    std::cout<<endl<<"-------partitioning operations: partition, partition_copy, is_partitioned, partition point--------"<<endl;
    
    auto partitionIt = partition(fwdListInts.begin(), fwdListInts.end(), bind(&CheckPosNeg<int>::IsNegative, CheckPosNeg<int>{}, placeholders::_1) );
    cout<<" fwd_list elems after partition negatives;positives: "<<*pivot<<endl;
    for(auto&& elem : fwdListInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    if(is_partitioned(fwdListInts.begin(), fwdListInts.end(), bind(&CheckPosNeg<int>::IsNegative, CheckPosNeg<int>{}, placeholders::_1)))
    {
        cout<<" partition_point: "<<*partition_point(fwdListInts.begin(), fwdListInts.end(), bind(&CheckPosNeg<int>::IsNegative, CheckPosNeg<int>{}, placeholders::_1))<<" partitionIt "<<*partitionIt<<endl;
    }
    
    auto fwdListSize = distance(fwdListInts.begin(), fwdListInts.end());
    //shuffle(fwdListInts.begin(), fwdListInts.end());
    list<int> listPos(fwdListSize/2 + 1), listNeg(fwdListSize/2 + 1);
    auto pairIter = partition_copy(fwdListInts.begin(), fwdListInts.end(), listPos.begin(), listNeg.begin(), CheckPosNeg<int>::IsPositive);
    cout<<" ranges after partition_copy positives/negatives: "<<endl;
    for(auto&& elem : listPos)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    for(auto&& elem : listNeg)
    {
        cout<<elem<<" ";
    }
    cout<<endl;

    std::cout<<endl<<"-------ordering operations: sort, partial_sort, is_sorted, is_sorted_until, nth_element--------"<<endl;
    vector<int> vecInts{9, -6, 10, -2, -8, -6, -7, -1, 9, 8, -1};
    for(auto&& elem : vecInts)
    {
        cout<<elem<<" ";
    }
    size_t nthElemIdx{4};
    cout<<endl<<" nth_element val "<<*next(vecInts.begin(), nthElemIdx)<<" on index "<<nthElemIdx<<" nth_element values "<<endl;
    nth_element(vecInts.begin(), next(vecInts.begin(), nthElemIdx), vecInts.end());
    cout<<" new nth_element val "<<*next(vecInts.begin(), nthElemIdx)<<endl;
    for(auto&& elem : vecInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    
    vecInts = {9, -6, 10, -3, -8, -6, -7, -1, 9, 7, -1};
    nthElemIdx = {7};
    cout<<endl<<" nth_element val "<<*next(vecInts.begin(), nthElemIdx)<<" on index "<<nthElemIdx<<" nth_element values "<<endl;
    nth_element(vecInts.begin(), next(vecInts.begin(), nthElemIdx), vecInts.end());
    cout<<" new nth_element val "<<*next(vecInts.begin(), nthElemIdx)<<endl;
    for(auto&& elem : vecInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl<<endl;
    
    vector<int> vecIota(11);
    iota(vecIota.begin(), vecIota.end(), -6);
    std::random_device rd;
    std::mt19937 g(rd());
    reverse(vecIota.begin(), vecIota.end());
    for(size_t idx{0}, vecSize{vecIota.size()}; idx < vecSize; ++idx)
    {
        if(vecIota[idx] %2 != 0)
        {
            ++vecIota[idx];
        }
    }
    for(auto&& elem : vecIota)
    {
        cout<<elem<<" ";
    }
    nthElemIdx = 7;
    cout<<endl<<" nth_element val "<<*next(vecIota.begin(), nthElemIdx)<<" on index "<<nthElemIdx<<" nth element values "<<endl;
    nth_element(vecIota.begin(), next(vecIota.begin(), nthElemIdx), vecIota.end());
    cout<<" new nth_element val "<<*next(vecIota.begin(), nthElemIdx)<<endl;
    for(auto&& elem : vecIota)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    
    std::cout<<endl<<"-------binary search operations: lower_bound_upper_bound, equal_range, binary_search--------"<<endl;
    auto bound_val{7};
    partition(vecInts.begin(), vecInts.end(), [&bound_val](const int elem){return elem <= bound_val;});
    for(auto&& elem : vecInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl<<endl;
    auto lowerBoundIt = lower_bound(vecInts.begin(), vecInts.end(), bound_val);
    cout<<"lower_bound(greater than or equal) "<<bound_val<<" than "<<*lowerBoundIt<<" at index "<<distance(vecInts.begin(), lowerBoundIt)<<endl;
    auto upperBoundIt = upper_bound(vecInts.begin(), vecInts.end(), bound_val);
    cout<<"upper_bound(strictly greater) "<<bound_val<<" than "<<*upperBoundIt<<" at index "<<distance(vecInts.begin(), upperBoundIt)<<endl;
    
    auto equalRangeVal{-1};
    partition(vecInts.begin(), vecInts.end(), [&equalRangeVal](const int elem){return elem < equalRangeVal;});
    for(auto&& elem : vecInts)
    {
        cout<<elem<<" ";
    }
    cout<<endl;
    auto pairIt = equal_range(vecInts.begin(), vecInts.end(), equalRangeVal);
    cout<<" "<<distance(vecInts.begin(), pairIt.first)<<" "<<distance(vecInts.begin(), pairIt.second)<<endl;
    for(auto iter = pairIt.first; iter != pairIt.second; ++iter)
    {
        cout<<" value: "<<*iter<<" index "<<distance(vecInts.begin(), iter)<<" ";
    }

    cout<<"-------------------------merge operations-----------------------"<<endl;
    std::vector<int> v{8, 2, -2, 0, 11, 11, 1, 7, 3};
    merge_sort(v.begin(), v.end());
    for (const auto& n : v)
        std::cout << n << ' ';
    std::cout<<endl;
    
    vector<int> vect{1, 2, 3, 4, 5, 6, -31, -23, -23, -14, -5};
    inplace_merge(vect.begin(), next(vect.begin(), 1+distance(vect.begin(), vect.end())/2), vect.end());
    for (const auto& elem : vect)
        std::cout << elem << ' ';
    std::cout<<endl;
    
    cout<<"-------------------------max heap operations------------------"<<endl;
    make_heap(vect.begin(), vect.end());
    for (const auto& elem : vect)
        std::cout << elem << ' ';
    std::cout<<endl;
    
    vect.push_back(9);
    push_heap(vect.begin(), vect.end());
    for (const auto& elem : vect)
        std::cout << elem << ' ';
    std::cout<<endl;
    
    vect.emplace_back(7);
    push_heap(vect.begin(), vect.end());
    for (const auto& elem : vect)
        std::cout << elem << ' ';
    std::cout<<endl;
    
    //works only with the last element
    vect.insert(vect.end(), {11, 12});
    push_heap(vect.begin(), vect.end());
    for (const auto& elem : vect)
        std::cout << elem << ' ';
    std::cout<<endl;
    
    cout<<"after pop_heap: "<<endl;
    pop_heap(vect.begin(), vect.end());
    for (const auto& elem : vect)
        std::cout << elem << ' ';
    std::cout<<endl;
    auto top = vect.back();
    vect.pop_back();
    cout<<"after pop_heap and pop_back: "<<endl;
    for (const auto& elem : vect)
        std::cout << elem << ' ';
    std::cout<<endl;
    
    sort_heap(vect.begin(), vect.end());
    cout<<"is_heap after sort_heap: "<<is_heap(vect.begin(), vect.end())<<endl;
    
    cout<<"-------------------------set operations-----------------------"<<endl;
    set<char> setCh{'a', 'c', 'd', 'e', 'g', 'h', 'i', 'j', 'k', 'l'};
    set<char> subsetCh{'c', 'd', 'h', 'i', 'k'};
    multiset<char> setForUnion{'b', 'b', 'c', 'c', 'e', 'f', 'h', 'h', 'i', 'm', '<', '@'};
    vector<char> unionSet(subsetCh.size()+setForUnion.size());
    cout<<"is subsetCh included in setCh: "<<includes(setCh.begin(), setCh.end(), subsetCh.begin(), subsetCh.end())<<endl;
    
    set_union(subsetCh.begin(), subsetCh.end(), setForUnion.begin(), setForUnion.end(), unionSet.begin());
    cout<<"set_union result: "<<endl;
    for (const auto& elem : unionSet)
        std::cout << elem << ' ';
    std::cout<<endl;
    
    list<char> intersectionSet(min(subsetCh.size(), setForUnion.size()));
    set_intersection(subsetCh.begin(), subsetCh.end(), setForUnion.begin(), setForUnion.end(), intersectionSet.begin());
    cout<<"set_intersection result: "<<endl;
    for (const auto& elem : intersectionSet)
        std::cout << elem << ' ';
    std::cout<<endl;
    
    forward_list<char> differenceSet(subsetCh.size() + setForUnion.size());
    set_difference(subsetCh.begin(), subsetCh.end(), setForUnion.begin(), setForUnion.end(), differenceSet.begin());
    cout<<"set_difference result: "<<endl;
    for (const auto& elem : differenceSet)
        std::cout << elem << ' ';
    std::cout<<endl;
    
    deque<char> symdifferenceSet(subsetCh.size() + setForUnion.size());
    set_symmetric_difference(subsetCh.begin(), subsetCh.end(), setForUnion.begin(), setForUnion.end(), symdifferenceSet.begin());
    cout<<"set_symmetric_difference result: "<<endl;
    for (const auto& elem : symdifferenceSet)
        std::cout << elem << ' ';
    std::cout<<endl;

    cout<<"-------------------------numeric operations-----------------------"<<endl;
    auto accPlus = accumulate(v.begin(), v.end(), 0, plus<int>{});
    auto accSumOfElemsInRange = accumulate(next(v.begin()), v.end(), v.front());//accumulate(v.begin(), v.end(), 0);
    auto prodOfElemsInRange = accumulate(next(v.begin()), v.end(), v.front(), multiplies<>{}); //accumulate((v.begin(), v.end(), 1, multiplies<>())
    cout<<"accPlus "<<accPlus<<" accSumOfElemsInRange "<<accSumOfElemsInRange<<" prodOfElemsInRange "<<prodOfElemsInRange<<endl;
    vector<int> vi{1, -2, 3, -4, 5, 1};
    auto diffElemsInRange = accumulate(next(vi.begin()), vi.end(), vi.front(), minus<int>{});
    vector<int> vi2{200, 100, 10, 2, 1, 1};
    double dividesElemsInRange = accumulate(next(vi2.begin()), vi2.end(), vi2.front(), divides<>{});
    cout<<"diffElemsInRange "<<diffElemsInRange<<" dividesElemsInRange "<<dividesElemsInRange<<" "<<minus<int>{}(3,2)<<endl;
    
    std::plus<int> plusInt;
    plusInt(2, 3);
    
    auto innerProd = inner_product(vi.begin(), vi.end(), vi2.begin(), 0);
    auto countPairsEqual = inner_product(vi.begin(), vi.end(), vi2.begin(), 0, plus<int>{}, equal_to<int>{});
    auto countDivisiblePairs = inner_product(vi.begin(), vi.end(), vi2.begin(), 0, plus<int>{}, [](const int& lhs, const int& rhs){return (lhs%rhs==0)||(rhs%lhs==0);});
    cout<<"innerProd "<<innerProd<<" countPairsEqual "<<countPairsEqual<<" countDivisiblePairs "<<countDivisiblePairs<<endl;
    
    auto adjDiffIt = adjacent_difference(vi.begin(), vi.end(), vi.begin());
    cout<<"vi after adjacent_difference between second and first elem in each pair of values "<<endl;
    for (const auto& elem : vi)
        std::cout << elem << ' ';
    std::cout<<endl;

    vector<int> partialSum(vi.size());
    auto partSumIt = partial_sum(vi.begin(), vi.end(), partialSum.begin());
    cout<<"partialSum of the above between the prev sum (stored in acc internally)  and next elem in vector "<<endl;
    for (const auto& elem : partialSum)
        std::cout << elem << ' ';
    std::cout<<endl;
    
    return 0;
}
