#include <iostream>
#include <type_traits>
#include <utility>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include <list>

/*
* C++11 language features
* 1. auto - the type of the variable that is being declared will be automatically deduced from its initializer. It may be accompanied by modifiers, 
*           such as const or &, which will participate in the type deduction.
*    decltype - Inspects the declared type of an entity or expression
*
* 2. delete and default specifiers for method's implementations. They show the implementation is not available or it defaults to the inherited behavior,
*   as in a default c-tor would call the default c-tor of the Base class (if any) and default c-tors of its members (if any). These c-tors must exist.
*
* 3. final - specified after a method's parameters list, indicates it is the final implementation for the method with that signature and cannot be further overriden.
*          - specified after a class name, marks it cannot be inherited from/extended/used as base class
*    override - specified after a method's parameters list, it marks the current implementation as an override of an inherited method
*
* 4. trailing return type: auto function_name(parameter_list) -> return_type. Mostly used for code's text alignemnt ot the left, which is variable
*                           due to distinct possible return types
*
* 5. r-value references: the declaration S&& D; declares D as an rvalue reference to the type determined by decl-specifier-seq S.
*                         - Rvalue references can be used to extend the lifetimes of temporary objects, while their value remains modifiable,
*                        unlike lvalue references to const that don't allow for value changes of the referred to temporary object.
*
*                         - More importantly, when a function has both rvalue reference and lvalue reference overloads, the rvalue reference overload 
*                        binds to rvalues (including both prvalues and xvalues), while the lvalue reference overload binds to lvalues. This allows 
*                        move constructors, move assignment operators, and other move-aware functions (e.g. std::vector::push_back()) to be automatically 
*                        selected when suitable.
*
*                        - Because rvalue references can bind to xvalues, they can refer to non-temporary objects, by using std::move on the latter, which is
*                        exactly equivalent to a static_cast to an rvalue reference type.
*
* 6. move semantics: move ctor and move assignment operator. They are implicitly generated if no other c-tor or d-tor is user provided.
*
*                     - The move constructor is typically called when an object is initialized (by direct-initialization or copy-initialization) from rvalue of 
*                    the same type, in the following scenarios: initialization, function argument passing, function return.
*
*                     - The move assignment operator is called whenever it is selected by overload resolution, e.g. when an object appears on the left-hand side of 
*                    an assignment expression, where the right-hand side is an rvalue of the same or implicitly convertible type. It can be merged with the copy
*                    assignment implementation, using a friend swap function, in the copy-and-swap idiom (rule of 4 and a half). The swap function can be used in
*                    implementing the move c-tor as well, if the default c-tor is called to construct *this, before performing swap.
*
*                    - Move semantics typically "steals" the resources held by the argument (e.g. pointers to dynamically-allocated objects, file descriptors, 
*                    sockets, I/O streams, running threads, etc.) rather than make copies of them, and leave the argument in some valid but, otherwise,
*                    indeterminate state. Although, the goal of move-semantics is to take the resources from another instance of the class, 
*                    leaving it in a state guaranteed to be assignable and destructible (well defined state).
*
*                    s(std::move(o.s)),       // explicit move of a member of class type (string, vector, MyClass etc)
*                    k(std::exchange(o.k, 0)) // explicit move of a member of non-class type (int, float, char etc)
*
*                   Syntax: MyClass(MyClass&&); MyClass& operator=(MyClass&&)
*
*  9. list initialization. - Direct initialization (T obj{arg}) and copy initialization (T obj; obj = {arg}). Direct list should be preferred to copy list initialization
*                          as it accepts explicit constructors
*                               - int a; / int a{} - default initialization
*                               - int a(5); / int a{5} - direct initialization
*                               - int a = 5; / int a = {5} - copy initialization
*                   
*                         - When a variable is list initialized using empty braces, value initialization takes place. In most cases, value initialization will 
*                         initialize the variable to zero. It does not happen the same with no argument paranethesis direct initialization. Moreover, this can create
*                         syntax confusion: int a() - is it a function named "a" that returns int or a variable of type int, named "a", directly initialized
*                         - Added benefit: “narrowing conversions” in list initialization are ill-formed. This means that if you try to brace initialize a variable 
*                                       using a value that the variable can not safely hold, the compiler is required to produce a diagnostic (usually an error).
*
* 12. nullptr
* 13. long long — target type will have width of at least 64 bits
*       In addition to: short — target type will be optimized for space and will have width of at least 16 bits. 
*                       long — target type will have width of at least 32 bits. 
*
* 14. char16_t — type for UTF-16 character representation. It has the same size, signedness, and alignment as std::uint_least16_t, but is a distinct type. 
*     char32_t — type for UTF-32 character representation. It has the same size, signedness, and alignment as std::uint_least32_t, but is a distinct type.  
*
* 15. type alias with using. Replaces typedef with a clearer syntax as the alias is lhs of = with the aliased type on the rhs of the
*                           using my_template_with_int = MyTemplate<int>;
*
* 18. generalized POD types: Specifies that the type is POD (Plain Old Data) type. This means the type is compatible with the types used in the C programming language,
*                            that is, can be exchanged with C libraries directly, in its binary form. 
*                            - is_pod<T>::value - defined in <type_traits>
*
*                            - scalar types (arithmetic types, enumeration, pointers, pointer-to-member types, std::nullptr_t, cv-qualified versions of these types) 
*                              are POD, as well as arrays of such types
*                            - POD classes: trivial class, standard-layout class, and has no non-static data members of type non-POD class (or array of such types).
*
* 19. lambda expressions - an unnamed function object capable of capturing variables in scope. 
*                        - [captured_args](args){body}. The captured args are captured as const values. They can be modified specifing "mutable" after (args).
*                        Also, by default, the captured args are captured by copy, so pointers and refs can be used. Since c++14, in capture clause there is available
*                        value initialization, especially used with std::move
*
*                        - dangling references: The C++ closures do not extend the lifetimes of objects captured by reference. Same applies to the lifetime of the 
*                        current *this object captured via this. 
*                        - If implicitly captured, the this pointer it is always captured by reference (specified this, not *this), even if the capture default is =.
*
* 20. attributes - Attributes provide the unified standard syntax for implementation-defined language extensions, such as the GNU and IBM language extensions 
*                   __attribute__((...)), Microsoft extension __declspec()
*
*                - An attribute can be used almost everywhere in the C++ program, and can be applied to almost everything: to types, to variables, to functions, 
*                  to names, to code blocks, to entire translation units, although each particular attribute is only valid where it is permitted by the implementation.
*                Syntax [[attribute_name]]: eg: [[noreturn]], [[gnu::always_inline, gnu::const]]
*
* 21. noexcept specifier - Specifies whether a function could throw exceptions. Potentially-throwing functions are: 
*                               - functions declared with noexcept specifier whose expression evaluates to false: void myfunc() noexcept(false)
*                               - functions declared without noexcept specifier, except for:
*                                       - destructors, unless the destructor of any base class or member data is potentially-throwing
*                                       - default constructors, copy constructors, move constructors that are implicitly-declared or defaulted on their first 
*                                       declaration in the inheritance chain, unless a base class c-tor or a data member is potentially-throwing
*                                       - copy assignment operators, move assignment operators that are implicitly-declared or defaulted on their first declaration
*                                       in the inheritance chain
*                                       - deallocation functions
*
*                       - If a virtual function is non-throwing, all declarations, including the definition, of every overrider must be non-throwing as well, 
*                         unless the overrider is defined as deleted. So, upon its first definition, if the virtual function has the noexcept specifier, all 
*                         subsequent overrides must have
*
* The noexcept operator performs a compile-time check that returns true if an expression is declared to not throw any exceptions. 
*
* 23. thread storage duration: The storage for the object is allocated when the thread begins and deallocated when the thread ends. Each thread has its own instance 
*                              of the object. Only objects declared thread_local have this storage duration. thread_local can appear together with static or 
*                              extern to adjust linkage.
*/


template<class T>
void CheckType(T&& t)
{
    auto tt = t;
    std::cout<<__func__<<" ";
    if(std::is_class<decltype(t)>::value)
        std::cout<<"-class"<<std::endl;
        
    if(std::is_compound<decltype(t)>::value)
        std::cout<<"-compound"<<std::endl;
}

// function returning , with C++11 trailing return type
auto fp(const std::string& str) -> size_t
{
    return str.size();
}

/*
* 7. scoped enums: strongly typed enums where each enumerator is encapsulated within the scope of the enumeration, and can be accessed only by using scope 
*                  resolution operator. There are no implicit conversions from the values of a scoped enumerator to integral types, although static_cast may 
*                  be used to obtain the numeric value of the enumerator. Thus, directly comparing 2 enumerators from 2 distinct enum classes is not possible,
*                  unlike the enumerators of unscoped enums.
*
*                  Syntax: enum class {Enumerator1, Enumerator2}; enum struct {}
*                  Both scoped and unscoped enums can have the underlying type fixed: enum MyEnum : unsigned {}
*/

enum UnscopedColor : uint_least16_t         //fixed width integer type as unnderlying type
{
    WHITE,
    BLACK,
    ORANGE
};

enum struct ScopedColor
{
    RED,
    GREEN,
    BLUE
};

/*
* 8. - constexpr - The constexpr specifier declares that it is possible to evaluate the value of the function or variable at compile time. 
*                Such variables and functions can then be used where only compile time constant expressions are allowed.
*                A constexpr variable must satisfy the following requirements:
*                   - its type must be a LiteralType
*                   - it must be immediately initialized
*                   - the full-expression of its initialization, including all implicit conversions, constructors calls, etc, must be a constant expression 
*
*                A constexpr function must satisfy the following requirements:
*                   - it must not be virtual (otherwise it has to be evaluated at runtime, contradicting the concept of constexpr)
*                   - it must not be a function-try-block (it involves jumps in the code, depending on runtime behavior)
*                   - for constructor and destructor(since C++20), the class must have no virtual base classes 
*                   - its return value (if any) and each of its parameters must be of a LiteralType
*                   - all its internally defined variables must be initialized
*                   - it can be a template
*                   - the function body must not contain: 
*                           - a goto statement
*                           - a statement with a label other than case and default 
*                           - a try-block
*                           - a definition of a variable of non-literal type (smart pointers, std::function, thread and sync mechansims etc)
*                           - a definition of a variable of static or thread storage duration 
*
* - literal types: Literal types are the types of constexpr variables and they can be constructed, manipulated, and returned from constexpr functions. 
*                A literal type is any of the following: 
*                    - scalar type ( arithmetic types, enumeration types, pointer types, pointer-to-member types, std::nullptr_t )
*                    - reference type;
*                    - an array of literal type; 
*                    - a class with a trivial d-tor
*                    - aggregate (no user defined c-tors - default or delete only, no base classes, no virtual methods) unions and structs with no variant members
*
*/


class ExampleConstexprMethod
{
    public:
    //if it weren't a template and t wasn't a ref to const: Composed' is not literal because it has data member 'mElements' of non-literal type 'std::list<int>'
    template<class T>
    constexpr std::string ConstexprFunc(int i, char c, std::string& str, const T& t)    
    {
        std::vector<T> vec;
        //int* ptr;             //warning: uninitialized variable in a constexpr function is a C++20 extension
        if(i > 0)
        {
            str+=c;
            
            std::cout<<str<<std::endl;
        }
        
        if(i < 0)
        {
            double d{3.14159};
            std::cout<<d<<std::endl;
        }
        
        auto lambda = []()
                    {
                        return std::string("Thread and mutex and smart pointer not allowed in constexpr func");
                    };
        
        vec.emplace_back(t);
        for(auto&& elem : vec)
        {
            std::cout<<elem<<std::endl;
        }
        
        //return vec; //if it weren't a template class: 'vector<Composed>' is not literal because it is not an aggregate and has no constexpr constructors 
                    //other than copy or move constructors
                    
        return lambda();
    }
};

/*
* 10. std::initializer_list<T>: It is a lightweight proxy object that provides access to an array of objects of type const T (allocated in read-only memory).
*                                - It is constructed from the list of elements as if the compiler generated and “array of N const T”, where N = number of elements 
*                                in the initializer list; this is called the initializer list’s backing array. 
*
*                                - Copying a std::initializer_list does not copy the backing array of the corresponding initializer list. 
*
*                                - Defined in header <initializer_list>, a std::initializer_list object is automatically constructed when:
*                                    - a braced-init-list is used to list-initialize an object, where the corresponding constructor accepts an 
*                                        std::initializer_list parameter,
*                                    - a braced-init-list is used as the right operand of assignment or as a function call argument, and the corresponding 
*                                       assignment operator/function accepts an std::initializer_list parameter,
*                                    - a braced-init-list is bound to auto, including in a ranged for loop. 
*/

class WithInitList
{
    public:
    WithInitList(const std::initializer_list<int>& iList):mList{iList}
    {
        std::cout<<"initializer_list c-tor"<<std::endl;
    }
    
    WithInitList(const std::list<int>& iList):mList{iList}
    {
        std::cout<<"list c-tor"<<std::endl;
    }
    
    std::list<int> GetList() const {return mList;};
    
    void PrintList() const
    {
        std::cout<<__func__<<std::endl;
        for(const int& elem : mList)
        {
            std::cout<<elem<<" ";
        }
        std::cout<<std::endl;
    }
    
    private:
    std::list<int> mList;
};

/*
* 11. - delegating constructors: in the member initializer list of a c-tor of a class, appears the call of a previously defined c-tor of the same class.
*                                The idea is to delegate some of the operations of the newly defined c-tor to the existing one ofr avoiding code redundancy,
*                                whilst the newly defined c-tor's implementation focuses on adding new operations. Not to be confused with calling Base class'
*                                c-tor. Only 1 c-tor has to be provided upon delegation and no further member must be initialized in the initializer list
*
*   - inherited constructors: If the using-declaration refers to a constructor of a direct base of the class being defined (e.g. using Base::Base;), 
*                           all constructors of that base (ignoring member access) are made visible to overload resolution when initializing the derived class. 
*
*                            - No specific c-tor overload is allowed in the using syntax.
*
*                            - It is useful when no c-tors are provided in the derived class or c-tor with other signatures are implemented, so it can safely reuse 
*                           the Base class c-tor(s) for inherited members initialization.
*
*                            - If overload resolution selects one of the inherited constructors when initializing an object of such derived class, then the Base 
*                           subobject from which the constructor was inherited is initialized using the inherited constructor, and all other bases and members 
*                           of Derived are initialized as by the defaulted default constructor
*/

class DelegateCtor
{
    public:
    DelegateCtor() = default;
    DelegateCtor(const int ii):i{ii}
    {
        std::cout<<" c-tor with int param "<<__func__<<std::endl;
    };
    
    DelegateCtor(const double dd):d{dd}
    {
        std::cout<<" c-tor with double param "<<__func__<<std::endl;
    };
    
    DelegateCtor(const int ii, const double dd):DelegateCtor{ii}
    {
        d = dd;
        std::cout<<" c-tor with 2 params "<<__func__<<std::endl;
    }
    
    private:
    int i;
    double d;
};

class UsingCtor : public DelegateCtor
{
    public:
    using DelegateCtor::DelegateCtor;   
    
    UsingCtor(const char cc):c{cc}          //requires default or c-tor with no params from delegateCtor, regardless using instruction above is specified
    {
        std::cout<<" c-tor with char param "<<__func__<<std::endl;
    }
    
    private:
    char c;
};

/*
* 16. variadic templates. Uses parameter pack as a placeholder for a series of elements of distinct types, whose size can vary, used in alias template, class template, 
*                         variable template(since C++14) and function template parameter lists.
*                         - Template arguments are deduced using template argument deduction.
*
*                         - In a primary class template, the template parameter pack must be the final parameter in the template parameter list. In a function template, 
*                         the template parameter pack may appear earlier in the list provided that all following parameters can be deduced from the function arguments.
*
*                         - A pattern followed by an ellipsis, in which the name of at least one parameter pack appears at least once, is expanded into zero or more 
*                         instantiations of the pattern, where the name of the parameter pack is replaced by each of the elements from the pack, in order.
*
*                         - Parameter pack can appear in lambda capture clause.
*/


template<class... Us>
void f(Us... pargs) {}

template<class... Ts>
void g(Ts... args)
{
    f(&args...); // “&args...” is a pack expansion
                 // “&args” is its pattern
}

//Parameter pack can appear in lambda capture clause.
template<class... Args>
void lambdaWithParamPack(Args... args)
{
    auto lm = [&, args...] { return g(args...); };
    lm();
}
//parameter pack with sizeof operator
template<typename... Ts>
void func(Ts... args)
{
    const int size = sizeof...(args) + 2;   //sizeof...(Types) works as well
    int res[size] = {1, args..., 2};        //use of parameter pack with list initalization
 
    // since initializer lists guarantee sequencing, this can be used to
    // call a function on each element of a pack, in order:
    int dummy[sizeof...(Ts)] = {(std::cout << args, 0)...};
}

/*
* 17. generalized unions - A union is a special class type that can hold only one of its non-static data members at a time. 
*                           - Just like in struct declaration, the default member access in a union is public, but it can be changed to private/protected.
*                          A union can have member functions (including constructors and destructors), but not virtual functions. A union cannot have base 
*                          classes and cannot be used as a base class. A union cannot have non-static data members of reference types. 
*
*                           - The union is at least as big as necessary to hold its largest data member, but is usually not larger. The other data members are 
*                          intended to be allocated in the same bytes as part of that largest member. The details of that allocation are implementation-defined, 
*                          except that all non-static data members have the same address. It is undefined behavior to read from the member of the union that 
*                          wasn't most recently written.
*                           - If members of a union are classes with user-defined constructors and destructors, to switch the active member, explicit destructor 
*                          and placement new are generally needed.
*/

union UnionExample
{
    std::int32_t n;     // occupies 4 bytes
    std::uint16_t s[2]; // occupies 4 bytes
    std::uint8_t c;     // occupies 1 byte
}; 

union ClassesMembersUnion
{
    std::string str;
    std::vector<int> vec;
    
    ~ClassesMembersUnion() {} // needs to know which member is active, only possible in union-like class 
};

/*
* 22. alignof(type-id) - returns size_t, that is the alignment, in bytes, required for any instance of the type indicated by type-id. Type-id is 
*                        either complete object type, an array type whose element type is complete, or a reference type to one of those types. 
*
*                      - CPU accesses memory in 2, 4, 8, 16, or 32 byte chunks at a time as accessing an address on 4-byte or 16-byte boundary is a lot faster 
*                      than accessing an address on 1-byte boundary. So, data alignment is a method to instruct the compiler to create data objects in memory on 
*                      specific byte boundaries for efficient data loads and stores to and from the processor. Consequently, every data/object type has the property 
*                      called alignment requirement, which is a nonnegative integer value (of type std::size_) and always a power of 2, indicating the address of that
*                      type should be a multiple of the alignment value. Hence, the address in memory of that data is aligned to a specific value (multiple of it)
*
*                      - the alignment is mostly same as its size of data type. Compiler aligns variables on their natural length boundaries. However, in order to 
*                      satisfy alignment requirements of all non-static members of a class/struct/union, padding bits may be inserted after some of its members, such 
*                      that they are aligned to the bytes of the member variable with highest size 
*                   
*                      - The alignment requirement of a type can be queried with alignof or std::alignment_of. The pointer alignment function std::align can be 
*                      used to obtain a suitably-aligned pointer within some buffer, and std::aligned_storage can be used to obtain suitably-aligned storage.  
*
* alignas - Each object type imposes its alignment requirement on every object of that type; stricter alignment (with larger alignment requirement) 
*/

struct AlignExample
{
    int i;
    char c;
    double d;
    float f[5];
};
