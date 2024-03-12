#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include <functional>
#include <memory>
#include <utility>
#include <tuple>
#include <array>
#include <map>
#include <cstdarg>
/*
* C++14 language features
*   1. character literal: char c = u8'character value'
*
*   2. made noexcept part of type system: - The noexcept-specification is a part of the function type and may appear as part of any function declarator.
*               eg: template<class T>
*                   constexpr auto Func(const T& value) noexcept -> shared_ptr<T>
*
*   3. new order of evaluation rules: 
*           eg: i = ++i + 2;       // well-defined
*               i = i++ + 2;       // undefined behavior until C++17,  In every simple assignment expression E1 = E2 and every compound assignment expression E1 @= E2,
*                                                   every value computation and side effect of E2 is sequenced before every value computation and side effect of E1.
*               f(i = -2, i = -2); // undefined behavior until C++17,  In a function call, value computations and side effects of the initialization of 
*                                           every parameter are indeterminately sequenced with respect to value computations and side effects of any other parameter.
*               f(++i, ++i);       // undefined behavior until C++17, unspecified after C++17
*               i = ++i + i++;     // undefined behavior, If a side effect on a memory location is unsequenced relative to another side effect on the same memory 
*                                                           location, the behavior is undefined. 
*               cout << i << i++; // undefined behavior until C++17
*               a[i] = i++;       // undefined behavior until C++17: In a subscript expression E1[E2], every value computation and side effect of E1 is 
*                                                                   sequenced before every value computation and side effect of E2.
*               n = ++i + i;      // undefined behavior, If a side effect on a memory location is unsequenced relative to a value computation using the value of 
*                                                       any object in the same memory location, the behavior is undefined. 
*
*   4. lambda capture of *this: - simple by-copy capture of the current object. Previously, it was captured this as simple by-reference capture of the current object.
*                               Having both versions in the same lambda capture leads to error, as any capture may appear only once, and its name must be different 
*                               from any parameter name
*           eg: [*this] {}; // until C++17: Error: invalid syntax
*                            // since C++17: OK: captures the enclosing S2 by copy
*
*   5. if constexpr: - if condition whose predicate's value can be evaluated at compile time. This means its branches (if, elseif, else) are evaluated at compile time
*                   with those not satisfying the predicated being eliminated at compile time. Specifically useful with type_traits struct verifiers used in template
*                   classes/functions for performing certain computation based on template type parameter's data type
*
*     constexpr lambdas: - Explicitly specifies that operator() is a constexpr function. If operator() satisfy all constexpr function requirements, operator() 
*                           will be constexpr even if constexpr is not present. 
*
*   6. Variables: 
*       6.1 Inline variable: The inline specifier, when used in a decl-specifier-seq of a variable with static storage duration (static class member or 
*                           namespace-scope variable), declares the variable to be an inline variable. A static data member declared constexpr on its 
*                           first declaration is implicitly an inline variable. 
*                           
*                           The inline specifier cannot be used with a function or variable(since C++17) declaration at block scope (inside another function).
*                           The inline specifier cannot re-declare a function or variable(since C++17) that was already defined in the translation unit as non-inline. 
*                           eg: // variable with external linkage included in multiple source files must be inline
*                               inline std::atomic<int> counter(0);
*
*                           The original intent of the inline keyword was to serve as an indicator to the optimizer that inline substitution of a function is 
*                           preferred over function call, that is, instead of executing the function call CPU instruction to transfer control to the function body,
*                           a copy of the function body is executed without generating the call. This avoids overhead created by the function call (passing the 
*                           arguments and retrieving the result) but it may result in a larger executable as the code for the function has to be repeated multiple 
*                           times. 
*
*                           A function defined entirely inside a class/struct/union definition, whether it's a member function or a non-member friend function, is 
*                           implicitly an inline function. 
*                   
*                           Since C++17, as the meaning of the keyword inline for functions came to mean "multiple definitions are permitted" rather than "inlining 
*                           is preferred", that meaning was extended to variables. Inline variables eliminate the main obstacle to packaging C++ code as header-only 
*                           libraries.
*
*                           There may be more than one definition of an inline function or variable(since C++17) in the program as long as each definition appears 
*                           in a different translation unit and (for non-static inline functions and variables(since C++17)) all definitions are identical. 
*                           For example, an inline function or an inline variable(since C++17) may be defined in a header file that is included in multiple source 
*                           files. Thus, inline const variables at namespace scope have external linkage by default.
*
*                           External linkage: The name can be referred to from the scopes in the other translation units. Variables and functions with external 
*                           linkage also have language linkage, which makes it possible to link translation units written in different programming languages. 
*                           Non-static variables declared at namespace scope have external linkage, likewise variables with extern specifier.
*
*       6.2 Structured bindings: Like a reference, a structured binding is an alias to an existing object. Unlike a reference, a structured binding does not have
*                                to be of a reference type. It works if the rhs structure is known at compile time.
*                                   Syntax: cv-auto ref-qualifier(optional) [ identifier-list ] = expression // uniform/direct initialization can also be used
*
*                                A structured binding declaration introduces all identifiers in the identifier-list as names in the surrounding scope and binds 
*                                them to subobjects or elements of the object denoted by expression. The bindings so introduced are called structured bindings.
*                                Nested structured bindings are not possible in c++17.
*
*                                A structured binding declaration first introduces a uniquely-named variable (here denoted by e) to hold the value of the initializer.
*                             >  If expression has array type and no ref-qualifier is present, then e is an array having the cv qualification of the structured 
*                                binding declaration. Then, the names in identifer-list are bound to the e's array elements. 
*                                   eg: int vec[2] {1, 2};
*                                       auto [x, y] = a;    // creates e[2], copies vec into e, then x refers to e[0], y refers to e[1]
*
*                             >  If the type of e is complete, non-union class type encapsulating a member called "value" and the number of identifiers is equal to
*                                "value", then the "tuple-like" binding protocol is used, each identifier being a reference to the corresponding tuple member,
*                                preserving the reference category (lvalue, rvalue), initializaed using get<i>(e). Thus, e is built as a tuple.
*                                   eg: float x{};
*                                       char  y{};
*                                       int   z{};
* 
*                                       std::tuple<float&, char&&, int> tpl(x, std::move(y), z);
*                                       const auto& [a, b, c] = tpl;    //a ahs type float&, b has type char&&, c has type int
*
*                             >  If the type of e is incomplete, then the names are bound to the accessible data members of it.
*                                   eg: struct S
*                                       {
*                                           mutable int x1 : 2;
*                                           volatile double y1;
*                                       };
*
*                                       const auto [x, y] = S{1, 2.3};  // x is an int lvalue identifying the 2-bit bit-field, y is a const volatile double lvalue
*
*       6.3 initializers for if and switch: If conditionally executes another statement and is used where code needs to be executed based on a run-time or 
*                                           compile-time(since C++17 with if constexpr) condition.
*                                               Syntax: if constexpr(optional) ( init-statement(optional) condition ) statement-true else statement-false
*                                   
*                                           Switch transfers control to one of several statements, depending on the value of a condition. 
*                                               Syntax:  switch ( init-statement(optional) condition ) statement 	
*
*       6.4 Guaranteed copy-ellision: Since C++17, a prvalue is not materialized until needed, and then it is constructed directly into the storage of its final 
*                                     destination. This sometimes means that even when the language syntax visually suggests a copy/move (e.g. copy initialization), 
*                                     no copy/move is performed — which means the type need not have an accessible copy/move constructor at all:
*                                       - initializing the returned object in a return statement, when the operand is a prvalue of the same class type 
*                                        (ignoring cv-qualification) as the function return type. The destructor of the type returned must be accessible at the 
*                                        point of the return statement and non-deleted, even though no T object is destroyed. 
*                                         eg: T f()
*                                           {
*                                               //T and U are related types, but distinct classes
*                                                return U{}; // constructs a temporary of type U, then initializes the returned T from the temporary
*                                           }
*                                           T g()
*                                           {
*                                               return T{}; // constructs the returned T directly; no move
*                                           }
*                                       - in the initialization of an object, when the initializer expression is a prvalue of the same class type 
*                                          (ignoring cv-qualification) as the variable type
*                                           eg: T x = T(f());
*
*       6.5  Temporary materialization: A prvalue of any complete type T can be converted to an xvalue of the same type T. This conversion initializes a 
*                                       temporary object of type T from the prvalue by evaluating the prvalue with the temporary object as its result object, 
*                                       and produces an xvalue denoting the temporary object. If T is a class or array of class type, it must have an accessible 
*                                       and non-deleted destructor. 
*                                           eg: struct S { int m; };
*                                               int i = S().m; // member access expects glvalue as of C++17; S() prvalue is converted to xvalue
*
*   7. Side Note: Value categories are rather properties of expression, than of values
*       7.1 glvalue: (generalized lvalue) an expression whose evaluation determines the identity of an object or function. So it can be lvalue or xvalue.
*                   It can have incomplete type, may be polymorphic and can eb converted to prvalue via implicit conversion
*
*       7.2 prvalue: (pure rvalue) is an expression that initializes an object or computes the value of an operand of built-in operators. The result object may 
*           be a variable, an object created by new-expression, a temporary created by temporary materialization, or a member.
*                eg. literals; function calls returning non-reference; address-of expression, this pointer; results of casts to non-reference, enumerators; lambdas;
*                    ternary operator returned values; non-type template parameter scalars; the result of accessing a struct/class non static method or enumerator
*
*       7.3 xvalue: (eXpiring value): is a glvalue that denotes an object whose resources can be reused
*               eg: the result of accessing a struct/class static data member, that can be accessed even via pointer; function calls returning rvalues; casts to
*                   rvalues; a[i] - array of rvalues; an object constructed after temporary materialization; move eligible expression
*
*       7.4 lvalue: is a glvalue that is not xvalue
*               eg: name of variable or a data member; a function; a variable of reference to rvalue type; a function call returning lvalue reference;
*                   a[i] - array of lvalues; string literals ("hello"); casts to lvalues
*
*       7.5 rvalue: is a prvalue or xvalue.It's address cannot be taken using address-of operator and can't be used as lhs in assignment expressions
*
*   8.Templates
*       8.1 Fold expressions: reduces (folds) parameter pack over a binary operator, such that the operator is applied to all parameters in pack, sequentially, 
*                            in (((arg1 operation arg2) operation arg3) operation arg4). The binary operator can be used with fol expression as condition for 
*                            enable_if_t.
*                           Syntax: (args operation...) - unary right fold
*                                   (...args operation) - unary left fold
*                                   (args operation...op init) - binary right fold
*                                   (op init...args operation) - binary left fold
*
*       8.2 Class template argument deduction: In order to instantiate a class template, every template argument must be known, but not every template argument type
*                                               has to be specified. Compiler can deduce the template types in certain situations:
*                                                   - new expressions: 
*                                                       auto y = new A{1, 2}; where template<class T> struct A {T mem1; T mem2;};
*
*                                                   - any declaration that specifies initialization of a variable and variable template, whose declared type is 
*                                                     the class template
*                                                       std::pair p(2, 4.5);     // deduces to std::pair<int, double> p(2, 4.5);
*                                                       std::tuple t(4, 3, 2.5); // same as auto t = std::make_tuple(4, 3, 2.5);
*                                                       std::less l;             // same as std::less<void> l;
*
*                                                       std::mutex mtx;
*                                                        {
*                                                            lock_guard lck(mtx);
*                                                        }
*
*                                                   - function cast expressions
*                                                       auto lck = lock_guard(mtx);
*
*                                           When performing template argument deduction for a template class instance, C, an additional fictional function template 
*                                           derived constructor C(C) is added, called the copy deduction candidate (as in copy c-tor counterpart). This function 
*                                           returns type C (the object to be created of class type C) and takes as template arguments the template parameters of the 
*                                           class, followed by the template parameters of the ctor (if any additional template params). Also, there is generated a
*                                           fictional template function for each specified ctor, if any, otherwise just one is generated, for available members.
*                                           It takes as arguments the arguments of the ctor (or available members if ctor is not defined), returns type C and
*                                           has similar template args as copy deduction candidate. These fictional constructors are public members of the hypothetical 
*                                           class type (the original class C extended with these 2 ctors at least). They are explicit if they were formed from an 
*                                           explicit constructor.
*
*                                           User defined deduction guides: have a relative similar syntax with trailing return type for functions, only that they
*                                           use class name instead of function name and suggest a possible candidate for template deduction. They are not found by 
*                                           name lookup and do not participate in overload resolution except for the overload resolution against other deduction guides 
*                                           Class template argument deduction of aggregates typically requires user-defined deduction guides. The deduction guides
*                                           do not have to be templates.
*                                               eg: template<class T>
*                                               A(const T&, const T&) -> A<T&>; // #3
* 
*                                               template<class T>
*                                               explicit A(T&&, T&&)  -> A<T>;  // #4
*
*                                               template<class T>
*                                               struct S
*                                               {
*                                                   S(T);
*                                               };
*                                               S(char const*) -> S<std::string>;
*
*                                           In overload resolution, partial ordering takes precedence over whether a function template is generated from a 
*                                           user-defined deduction guide: if the function template generated from the constructor is more specialized than the one 
*                                           generated from the user-defined deduction guide, the one generated from the constructor is chosen. In case of tie-break
*                                               - A function template generated from a user-defined deduction guide is preferred over one implicitly generated from 
*                                               a constructor or constructor template.
*                                               - The copy deduction candidate is preferred over all other function templates implicitly generated from a constructor
*                                               or constructor template. 
*
*       8.3 auto template non-type parameters: the type of the template non-type parameter is the type deduced from the variable used as template argument.
*                                               If a deduced parameter type is not a structural type, the program is ill-formed (integer, lvalue reference type,
*                                               pointer, pointer to member, enumeration type. Floating point and literals only starting with c++20).
*                                                   template<auto n>
*                                                   struct B {  ...  };
* 
*                                                   B<5> b1;   // OK: non-type template parameter type is int
*                                                   B<'a'> b2; // OK: non-type template parameter type is char
*
*   9. Namespaces: - simplified access within nested namespaces
*                   eg: namespace A 
*                           { namespace B 
*                               { int t}
*                           }
*                       A::B::i = 3;
*                   - using declaration declaring multiple names 
*                           using declarator-list ;
*                           using A::g, A::f; // (C++17) OK: double declaration allowed at namespace scope
*/

//4. capture by value *this in lambda
class MyClass
{
    public:
    void Wrapper()
    {
        //capture the pointer
        [this](){this->i = 7; this->str = "inside lambda";}();
    }
    
    void SecondWrapper()
    {
        //capture the instance, by value, as this is dereferenced
        [*this]() mutable {this->i = 8; this->str = " second wrapper inside lambda";}();
    }
    
    int GetI() const {return i;};
    std::string GetStr() const {return str;};
    
    private:
    int i;
    std::string str;
};

//5. if constexpr + lambda constexpr
template <class T>
constexpr void FunctionWithConstexpr(T&& input)
{
    //type_traits needs a type parameter. RValue and lvalue are value categories for expressions.
    //is_integral needs a non-reference type to return true. is_integral_v<int &&> (rvalue reference) still returns false, because it is a reference type
    if constexpr (std::is_floating_point<typename std::remove_reference<T>::type>::value)
    {
        std::cout<<__func__<<" "<<__LINE__<<" ";
        std::cout<<std::abs(input)<<std::endl;
    }
    else if constexpr (std::is_integral<typename std::remove_reference<T>::type>::value)
    {
        std::cout<<__func__<<" "<<__LINE__<<" ";
        [&input]() constexpr
        {
            ++input;
            std::cout<<" pre increment int "<<input<<std::endl;
        }();
    }
    else if constexpr(/*std::is_function<T>::value*/ std::is_invocable<T>::value)
    {
        //Types like std::function, lambdas, classes with overloaded operator() and pointers to functions don't count as function types =>
        //is_function returns 0. Since c++17 is_invocable can be used
        std::cout<<__func__<<" "<<__LINE__<<" invocable ";
        input();
    }
    else if constexpr(std::is_null_pointer<T>::value)
    {
        std::cout<<__func__<<" "<<__LINE__<<" ";
        input = new T{};
    }
    else if constexpr(std::is_pointer<T>::value)
    {
        std::cout<<__func__<<" "<<__LINE__<<" ";
        std::cout<<"preincrement dereferenced ptr value: "<<++*input<<std::endl;
    }
}

//6.4 guaranteed copy ellision
struct T 
{
    T()
    {
        std::cout<<"T ctor"<<std::endl;
    }
    
    ~T()
    {
        std::cout<<"T dtor"<<std::endl;
    }
    
    T(const T&)
    {
        std::cout<<"T copy ctor"<<std::endl;
    }
    
    T(T&&)
    {
        std::cout<<"T move ctor"<<std::endl;
    }
};

struct U : public T
{
    U()
    {
        std::cout<<"U ctor"<<std::endl;
    }
    
    ~U()
    {
        std::cout<<"U dtor"<<std::endl;
    }
    
    U(const U&)
    {
        std::cout<<"U copy ctor"<<std::endl;
    }
    
    U(U&&)
    {
        std::cout<<"U move ctor"<<std::endl;
    }
};

T funcU()
{
    //T and U are distinct types
    return std::move(U{}); // constructs a temporary of type U, then initializes the returned T from the temporary => copy/move not ellided
}
T funcT()
{
    return T{}; // constructs the returned T directly; no move
}
T func2U()
{
    U u{};
    //T and U are distinct types
    return u; // constructs a temporary of type U, then initializes the returned T from the temporary => copy/move not ellided
}
T func2T()
{
    T t{};
    return t; // constructs the returned T directly; no move
}

//6.5 Temporary materialization
struct TempMater
{
    TempMater(const int ii):i{ii}{};
    
    int i;  
};

//8.1 Fold expressions
template<class ... Args>
void PrintArgs(Args&& ... args)
{
    (std::cout<<...<<args)<<std::endl;
}

//fold expression with unary operators
template<class ... Args>
int AndArgs(Args&& ... args)
{
    return (...&& args);
}

template<class ... Args>
int OrArgs(Args&& ... args)
{
    return (args || ...);
}

//fold expression with binary operator + fold expression used in template parameter pack, with enable_if_t
//it is possible to have more than 1 enable_if_t as template parameter
template<class T, class ... Args, std::enable_if_t<(std::is_integral<T>::value || std::is_floating_point<T>::value), bool> = true,
                                  std::enable_if_t<(std::is_integral_v<Args> && ...) || (std::is_floating_point_v<Args> && ...), bool> = true>
T SumArgs(T init, Args&& ... args)
{
    return (init + ... + args);
}

template<class T, class ... Args, typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, bool>::type = true>
T ProdArgs(T init, Args&& ... args)
{
    return (args * ... * init);
}

template<class ... Args, std::enable_if_t<((std::is_integral_v<Args> && ...)), bool> = true>
int DiffArgs(int init, Args&& ... args)
{
    return (init - ... - args);
}

//fold expression with STL container operation
template<class T, class ... Args>
void VecPushBack(T first, Args&& ... args)
{
    std::vector<T> vecArgs;
    vecArgs.push_back(first);
    std::cout<<"push_back strings in vector"<<std::endl;
    (vecArgs.push_back(std::forward<Args>(args)),...);
    
    for(auto&& elem : vecArgs)
    {
        std::cout<<elem<<" ";
    }
    std::cout<<std::endl;
}

//fold expression with lambda init capture
template<class ... Args>
void LambdaCall(Args&& ... args)
{
    size_t tuple_size = sizeof...(Args);
    [lambda_args = std::make_tuple(std::forward<Args>(args)...), tuple_size]()
    {
        std::cout<<tuple_size<<std::endl;
    }();
}


//8.2 template argument deduction
template<class A, class B>
struct AggregateStruct
{
    A a;
    B b;
    
    //if c-tor is provided, no user guided template deduction is necessary
    //AggregateStruct(const A ca, const B cb):a{ca}, b{cb}{};
};

//deduction guide specifying how initialization should deduced parameter types for this template
template<class A, class B>
AggregateStruct(A, B) -> AggregateStruct<A, B>; 
//const char* instantiations will be deduced as template with std::string template args
AggregateStruct(const char*, const char*) -> AggregateStruct<std::string, std::string>;


template<class T>
class TemplateClass
{
    public:
    explicit TemplateClass(const T& mem) noexcept : member{mem}
    {};
    
    TemplateClass(T&& arg, ...)
    {
        // va_list found in <cstdarg> and list is its type, used to iterate on ellipsis 
         va_list argsList; 
  
        // Initialize position of va_list 
        va_start(argsList, arg); 
        
        member = arg;
        auto tupleVarArgs = std::make_tuple(va_arg(argsList, typename std::remove_reference<T>::type));
        mTupleSize = std::tuple_size_v<decltype(tupleVarArgs)>;
        
        // Ends the use of va_list 
        va_end(argsList); 
    }
    
    T GetMember() const {return member;}
    
    private:
    T member;
    size_t mTupleSize{};
};












