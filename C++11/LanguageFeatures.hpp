#include <iostream>
#include <type_traits>
#include <utility>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <functional>
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
*                        Rvalue references can be used to extend the lifetimes of temporary objects, while their value remains modifiable,
*                        unlike lvalue references to const that don't allow for value changes of the referred to temporary object.
*                        More importantly, when a function has both rvalue reference and lvalue reference overloads, the rvalue reference overload 
*                        binds to rvalues (including both prvalues and xvalues), while the lvalue reference overload binds to lvalues. This allows 
*                        move constructors, move assignment operators, and other move-aware functions (e.g. std::vector::push_back()) to be automatically 
*                        selected when suitable.
*                        Because rvalue references can bind to xvalues, they can refer to non-temporary objects, by using std::move on the latter, which is
*                        exactly equivalent to a static_cast to an rvalue reference type.
*
* 6. move semantics: move ctor and move assignment operator. They are implicitly generated if no other c-tor or d-tor is user provided.
*                    The move constructor is typically called when an object is initialized (by direct-initialization or copy-initialization) from rvalue of 
*                    the same type, in the following scenarios: initialization, function argument passing, function return.
*                    The move assignment operator is called whenever it is selected by overload resolution, e.g. when an object appears on the left-hand side of 
*                    an assignment expression, where the right-hand side is an rvalue of the same or implicitly convertible type. It can be merged with the copy
*                    assignment implementation, using a frind swap function, in the copy-and-swap idiom (rule of 4 and a half). The swap function can be used in
*                    implementing the move c-tor as well, if the default c-tor is called to construct *this, before performing swap.
*
*                    Move semantics typically "steals" the resources held by the argument (e.g. pointers to dynamically-allocated objects, file descriptors, 
*                    sockets, I/O streams, running threads, etc.) rather than make copies of them, and leave the argument in some valid but, otherwise,
*                    indeterminate state. Although, the goal of move-semantics is to take the resources from another instance of the class, 
*                    leaving it in a state guaranteed to be assignable and destructible (well defined state).
*
*                    s(std::move(o.s)),       // explicit move of a member of class type (string, vector, MyClass etc)
*                    k(std::exchange(o.k, 0)) // explicit move of a member of non-class type (int, float, char etc)
*
*                   Syntax: MyClass(MyClass&&); MyClass& operator=(MyClass&&)
*
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
* 8. constexpr - The constexpr specifier declares that it is possible to evaluate the value of the function or variable at compile time. 
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
* literal types: Literal types are the types of constexpr variables and they can be constructed, manipulated, and returned from constexpr functions. 
*                A literal type is any of the following: 
*                    - scalar type ( arithmetic types, enumeration types, pointer types, pointer-to-member types, std::nullptr_t )
*                    - reference type;
*                    - an array of literal type; 
*                    - a class with a trivial d-tor
*                    - aggregate (no user defined c-tors - default or delete only, no base classes, no virtual methods) unions and structs with no variant members
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
