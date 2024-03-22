/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <complex>
#include <array>
#include <sstream>
#include <chrono>
#include <vector>
#include <random>

#include "LibraryFeatures.hpp"
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

    std::cout<<"----------------------Library features"<<std::endl;
    std::cout<<"----------------------std::any examples"<<std::endl;
    auto a0 = std::make_any<std::string>("Hello, std::any!\n");
    auto a1 = std::make_any<std::complex<double>>(0.1, 2.3);
 
    std::cout << std::any_cast<std::string>(a0);
    //cast to reference value category (property of expressions)
    std::cout << std::any_cast<std::complex<double>&>(a1) <<std::endl;
    //cast to ptr 
    auto any_ptr = std::any_cast<std::string>(&a0);
    std::cout<<*any_ptr<<std::endl;
    //cast to reference to r-value
    std::string str{"example string"};
    auto rvalueref_any = std::any_cast<std::string&&>(std::move(str));
    std::cout<<"####"<<str<<" %%%% "<<rvalueref_any<<std::endl;
    
    std::any anyVar{5};
    std::cout<<"any value: "<<std::any_cast<int>(anyVar)<<" "<<anyVar.type().name()<<" "<<anyVar.has_value()<<std::endl;
    anyVar = 3.14159;
    std::cout<<"any value after operator=: "<<std::any_cast<double>(anyVar)<<" "<<anyVar.type().name()<<" "<<anyVar.has_value()<<std::endl;
    anyVar.emplace<std::string>(std::string("const char*"));
    std::cout<<"any value after emplace<T>: "<<std::any_cast<std::string>(anyVar)<<" "<<anyVar.type().name()<<" "<<anyVar.has_value()<<std::endl;
    anyVar.reset();
    
    std::cout<<"----------------------std::charconv examples"<<std::endl;
    char buffer[10];
    int i{1234567890};
    
    auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), i);
    if(ec == std::errc())
    {
        std::cout<<"std::to_chars"<<" "<<*ptr<<" "<<buffer<<std::endl;
    }
        
    stringstream sstream;
    sstream <<i;
    std::cout<<"stringstream "<<sstream.str()<<std::endl;
    
    std::string strFloat{"3.14159"};
    float pi{};
    
    std::from_chars_result result = std::from_chars(strFloat.c_str(), strFloat.c_str() + strFloat.size(), pi);
    if(result.ec == std::errc())
    {
        std::cout<<pi<<" "<<*result.ptr<<" "<<pi*2<<std::endl;
    }
    
    stringstream sstream2{strFloat};
    float pi2{};
    sstream2 >> pi2;
    std::cout<<"stringstream float "<<pi2<<std::endl;
    
    std::cout<<"----------------------std::execution examples"<<std::endl;
    std::vector<int> vecInt{9000, -33, 4, 1, 0, -47, 2020, 4007, 4, 5, 2, 101, 1001, 100, 99};
    auto startTime = std::chrono::steady_clock::now();
    std::sort(std::execution::par, vecInt.begin(), vecInt.end());
    auto endTime = std::chrono::steady_clock::now();
    std::cout<<"parallel sort time: "<<std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count()<<std::endl;
    
    startTime = std::chrono::steady_clock::now();
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vecInt.begin(), vecInt.end(), g);
    endTime = std::chrono::steady_clock::now();
    std::cout<<"shuffle time: "<<std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count()<<std::endl;
    
    startTime = std::chrono::steady_clock::now();
    std::sort(std::execution::unseq, vecInt.begin(), vecInt.end());
    endTime = std::chrono::steady_clock::now();
    std::cout<<"unseq sort time: "<<std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count()<<std::endl;
    std::shuffle(vecInt.begin(), vecInt.end(), g);
    
    startTime = std::chrono::steady_clock::now();
    std::sort(std::execution::seq, vecInt.begin(), vecInt.end());
    endTime = std::chrono::steady_clock::now();
    std::cout<<"seq sort time: "<<std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count()<<std::endl;
    std::shuffle(vecInt.begin(), vecInt.end(), g);
    
    startTime = std::chrono::steady_clock::now();
    std::sort(std::execution::par_unseq, vecInt.begin(), vecInt.end());
    endTime = std::chrono::steady_clock::now();
    std::cout<<"par_unseq sort time: "<<std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count()<<std::endl;
    std::shuffle(vecInt.begin(), vecInt.end(), g);
    
    startTime = std::chrono::steady_clock::now();
    auto pivot = vecInt[8]; 
    std::cout<<"pivot: "<<pivot<<std::endl;
    std::partition(std::execution::par_unseq, vecInt.begin(), vecInt.end(), [pivot](const int val){return val<pivot;});
    endTime = std::chrono::steady_clock::now();
    std::cout<<"par_unseq partition time: "<<std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count()<<std::endl;
    for(auto elem : vecInt)
    {
        cout<<elem<<" ";
    }
    std::cout<<std::endl;
    std::shuffle(vecInt.begin(), vecInt.end(), g);
    
    startTime = std::chrono::steady_clock::now();
    pivot = vecInt[10];
    std::cout<<"pivot: "<<pivot<<std::endl;
    std::partition(std::execution::par_unseq, vecInt.begin(), vecInt.end(), [pivot](const int val){return val<pivot;});
    endTime = std::chrono::steady_clock::now();
    std::cout<<"unseq partition time: "<<std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count()<<std::endl;
    for(auto elem : vecInt)
    {
        cout<<elem<<" ";
    }
    std::cout<<std::endl;
    std::shuffle(vecInt.begin(), vecInt.end(), g);
    
    std::cout<<"----------------------std::filesystem examples"<<std::endl;
    std::filesystem::path file_path{R"(C:\Users\scretu\Downloads\test.docx)"};
    
    std::cout<<"root_name: "<<file_path.root_name()<<" root_DIR: "<<file_path.root_directory()<<" root_path "<<file_path.root_path()<<endl;
    std::cout<<"relative_path: "<<file_path.relative_path()<<" parent_path: "<<file_path.parent_path()<<" filename() "<<file_path.filename()<<endl;
    std::cout<<"stem: "<<file_path.stem()<<" extension: "<<file_path.extension()<<" empty "<<file_path.empty()<<endl;
    
    const std::filesystem::path from{"/none1/a"}, to{"/none2/b"};
    try
    {
        std::filesystem::copy_file(from, to); // throws: files do not exist
    }
    catch (std::filesystem::filesystem_error const& ex)
    {
        std::cout <<endl<< "what():  " << ex.what() << '\n'
                  << "path1(): " << ex.path1() << '\n'
                  << "path2(): " << ex.path2() << '\n'
                  << "code().value():    " << ex.code().value() << '\n'
                  << "code().message():  " << ex.code().message() << '\n'
                  << "code().category(): " << ex.code().category().name() << '\n';
    }
        std::cout<<std::filesystem::current_path()<<std::endl;
    
    std::filesystem::path dir_path{"C:\\Users\\me\\Downloads"};
    if(std::filesystem::create_directory(dir_path))
    {
        std::filesystem::directory_entry dir_entry{dir_path};
    
        std::cout<<dir_entry.is_regular_file()<<" "<<dir_entry.is_directory()<<" "<<dir_entry.is_other()<<" "<<dir_entry.is_socket()<<" "<<dir_entry.is_fifo()<<std::endl;
        std::cout<<dir_entry.is_character_file()<<" "<<dir_entry.is_block_file()<<" "<<dir_entry.is_symlink()<<" "<<dir_entry.exists()<<std::endl;
    }
    
    std::filesystem::create_directories(dir_path/"dir1"/"dir2"/"dir3");
    for(const std::filesystem::directory_entry&  dir_entry : std::filesystem::directory_iterator{dir_path})
    {
        std::cout<<dir_entry.path()<<" ";
    }
    std::cout<<std::endl;
    for(const std::filesystem::directory_entry&  dir_entry : std::filesystem::recursive_directory_iterator{dir_path})
    {
        std::cout<<dir_entry.path()<<" ";
    }
    
    
    std::fstream file_stream{"myfile.txt"};
    file_stream<<"write some text to file";
    
    std::filesystem::file_status file_status = std::filesystem::status("myfile.txt");
    std::cout<<std::endl<<static_cast<int>(file_status.type())<<std::endl;

    std::cout<<"--------------------------------std::pmr memory_resource examples"<<std::endl;
    /*
    *   The program measures the time of creating huge double-linked lists using the following allocators:
    *       - default standard allocator,
    *       - default pmr allocator,
    *       - pmr allocator with monotonic resource but without explicit memory buffer,
    *       - pmr allocator with monotonic resource and external memory buffer (on stack). 
    */
    
    constexpr int iterations{100};
    constexpr int total_nodes{2'00'000};
    
    auto standard_list_allocator = [total_nodes]
                                    {
                                        std::list<int> mList;
                                        for(size_t idx{}; idx < total_nodes; ++idx)
                                        {
                                            mList.push_back(idx);
                                        }
                                    };
                                    
    auto pmr_list_allocator = [total_nodes]
                                {
                                    std::pmr::list<int> mPmrList;
                                    for(size_t idx{}; idx < total_nodes; ++idx)
                                    {
                                        mPmrList.push_back(idx);
                                    }
                                };
                                
    auto pmr_alloc_no_buffer = [total_nodes]
                                {
                                    //create the memory_resource of type monotonic_buffer_resource
                                    std::pmr::monotonic_buffer_resource mbr{};
                                    //use it to construct the polymorphic allocator
                                    std::pmr::polymorphic_allocator<int> polyAlloc{&mbr};
                                    
                                    std::pmr::list<int> mPmrList{polyAlloc};
                                    for(size_t idx{}; idx < total_nodes; ++idx)
                                    {
                                        mPmrList.push_back(idx);
                                    }
                                };
                                
    auto pmr_alloc_buffer = [total_nodes]
                                {
                                     std::array<std::byte, total_nodes * 32> buffer{};
                                    //create the memory_resource of type monotonic_buffer_resource
                                    std::pmr::monotonic_buffer_resource mbr{buffer.data(), buffer.size()};
                                    //use it to construct the polymorphic allocator
                                    std::pmr::polymorphic_allocator<int> polyAlloc{&mbr};
                                    
                                    std::pmr::list<int> mPmrList{polyAlloc};
                                    for(size_t idx{}; idx < total_nodes; ++idx)
                                    {
                                        mPmrList.push_back(idx);
                                    }
                                };
                                
    std::cout<<"--------------------------------std::optional examples"<<std::endl;
                                
    std::optional<std::unique_ptr<int>> optUPtrInt = AllocateUPtr(8);
    if(optUPtrInt) //optUPtrInt.has_value()
    {
        std::cout<<"std::option value "<<**optUPtrInt<<" "<<*optUPtrInt.value()<<std::endl;
    }
    
    int i{99};
    SetValue(i);
    SetValue(nullptr); 
    
    std::cout<<"--------------------------------std::string_view examples"<<std::endl;
    std::string str{"string example"};
    std::string_view strV{"string_view example with char underlying type"};
    
    std::cout<<str[7]<<" "<<strV[7]<<std::endl;
    
    str[7] = '7';
    //strV[7] = '7';    //compile time error: assignemnt of read only location
    int res = strV.find("with");
    int res2 = strV.rfind("ing");
    std::cout<<"string_view find and rfind "<<res<<" "<<res2<<std::endl;
    
    std::cout<<"--------------------------------std::variant examples"<<std::endl;
    
    std::variant<int, char, std::string, std::string_view, double> variantInst{std::string("string")};
    std::cout<<"variant field "<<std::get<std::string>(variantInst)<<" and index "<<variantInst.index()<<std::endl;
    
    variantInst.emplace<char>('z');
    std::cout<<"variant.emplace<type> "<<std::get<1>(variantInst)<<" "<<variantInst.index()<<std::endl;
    
    variantInst.emplace<4>(3.14159);
    std::cout<<"variant.emplace<index> "<<std::get<double>(variantInst)<<" "<<variantInst.index()<<std::endl;
    
    auto newVariant = variantInst;
    newVariant.emplace<int>(2);
    std::vector<std::variant<int, char, std::string, std::string_view, double>> vecVariants{variantInst, newVariant};
    
    for(auto&& vari : vecVariants)
        std::visit([](auto&& active_variant_field){std::cout<<"->visit variant "<<active_variant_field<<std::endl;}, vari);
        
    for(auto&& vari : vecVariants)
    {
        //a struct with 3 overloaded operator()'s. The appropiate overload is chosen depending on variant's active field
        std::visit(CallableWrapper{ [](const auto& elem){std::cout<<"CallableWrapper: "<<elem<<std::endl;},
                                    [](int arg){std::cout<<"CallableWrapper int: "<<arg<<" "<<++arg<<std::endl;},
                                    [](double arg){std::cout<<"CallableWrapper double: "<<arg+M_PI<<std::endl;}
                                  },                                                                 
                    vari);
    }
        
    std::cout<<"--------------------------------weak_from_this() examples"<<std::endl;    
    std::shared_ptr<MyClass> sptr = std::make_shared<MyClass>();
    sptr->ShMethod();
    sptr->WkMethod();
    
    std::cout<<"--------------------------------shared_ptr::operator[] examples"<<std::endl;
    std::shared_ptr<int[]> spVec{new int[5]{1,2,3,4,5}};
    
    spVec[0] = -1;
    ++spVec[2];
    --spVec[3];
    
    std::shared_ptr<char[]> spChar;
    
    spChar = std::shared_ptr<char[]>(new char[3]);
    spChar[0] = 'e';
    spChar[1] = 'k';
    spChar[2] = 'd';
    
    for(size_t idx{}; idx < 5; ++idx)
    {
       std::cout<<spVec[idx]<<" ";
    }
    
    std::cout<<std::endl<<"--------------------------------std::tuple examples"<<std::endl;
    //template type deduction on instantiation
    std::tuple myTup{std::string("str"), 1, true, 2.7182};
    //template type deduction on return statement
    std::tuple result = GetTuple();
    
    auto makeFromTuple = std::tuple(1, 2.2, 'a');
    Example ex = std::make_from_tuple<Example>(makeFromTuple);
    
    std::cout<<"struct inst members after make_fom_tuple "<<ex.mi<<" "<<ex.md<<" "<<ex.mc<<std::endl;
    
    auto add_lambda = [](auto first, auto second) { return first + second; };
    std::cout<<"std apply with lambda and std pair "<<std::apply(add_lambda, std::pair(2.0f, 3.0f))<<std::endl;

    return 0;
}
