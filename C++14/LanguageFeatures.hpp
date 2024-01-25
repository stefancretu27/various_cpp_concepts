#include <iostream>
#include <memory>
using namespace std;

/* C++14 language features
* 1. Variable Template: defines a family of variables or static data members => the type of the data is generic (the template type parameter)
*                        The variable or the static member instantiated from the template are called instantiated variable/ static member.
*                        For the static variable template member, likewise any static data member, it has to be explicitly defined outside
*                        the class declaration. The definition reuses the generic tempplate parameter type as it doesn't require explicit
*                        instantiation of the template.
*                        The same variable name can be reused for multiple types, memory being set aside separately for each type, once.
*                        Reusing the variable template with an already instantiated type, would rewrite it's previous value, upon assignment.
*                        Declaring a non template variable with the same name as the template variable, is not conflicting, but also doesn't
*                        represent an explicit instantiation. It is just another variable.

* 2. Generic lambdas:  A lambda expression that has at least one parameter of auto type. Thus, the parameter's type is inferred at compile time, 
*                      from the function's instantiation argument's type. Conceptually, a generic lambda is equivalent to a function object with 
*                      a templatized function-call operator method, whose arguments have the type defined by the template type parameter.
*
* 3. Capture-init for lambdas: Data captured in the capture list of a lambda can be initialized inside the capture list.
*                              Works with variables captured by value, not by reference. The initialization is valid only for the lambda's scope.
*
* 4. New-delete ellision: One such case is when the lifetime of an object allocated with new or whne it is deleted, is nested within the lifetime of another 
*                         object allocated with new/deleted, resulting in one call to new/delete. It is also called allocation elision and is the second form 
*                         of optimization after copy/move elision introduced in c++11, which results in zero copy pass by value operations 
*                         (return by value, pass argument by value), as the object is built in place where it would have been copied/moved to.
*
* 6. Binary literals: Used to initialize data with a bianry sequence, as done in decimal and hexa.
*                     Digit separators = commas used to separate digits of numeric literals, ignored by the compiler. Also, works with floating point numbers."<<endl;
*/



class VariableTemplate
{
    public:
        template<class T>
        static T GetStaticTemplVariable() 
        {
            return myTemplVariable<T>;
        };
        
        template<class T>
        static void SetStaticTemplVariable(const T& data)
        {
            myTemplVariable<T> = data;
        };
        
    private:
        //static variable template member
        template<class T>
        static T myTemplVariable;
};

//explicit definition of the static member, as usual
template<class T>
T VariableTemplate::myTemplVariable;

//declaration of variable template
template<class T>
T myGenericVariable;

template<class T>
T anotherGenericVar = {};

/*
* 5. Relaxed restrictions on constexpr functions: The constexpr qualifier was introduced in c++11 to indicate a compile time known constant data. 
*                                                Furthermore, it could also be used as a qualifier for functions and methods, if they satisfy a series of criteria, as follows:
*                                                    - they are not functions-try-blocks: functions whose bodies are try-block. It also holds for c-tors.
*                                                    - the returned value is a Literal, likewise its parameters
*                                                    - for constexpr c-tor, the class must have non virtual base class
*                                                    - the function's body is either deleted or contains only: static-assert, using, typedef, maximum one return statement, switch-case
*                                                    - the function's body mustn't contain: try-catch and go to statements (as they lead to jumps in code), non-literal
*                                                      variables definitions, thread local data definition, static data definition
*    
*                                                The relaxation measures include:
*                                                   - allow loop statements: for, while, do while
*                                                   - allow if statements
*                                                   - allow variables declarations, but not {uninitialized variables, static, thread local}. Variables must be initialzied with
*                                                      literals, const data (compile time known values) or default/uniform initialized.
*   
*                                                 Function try-blocks are often used to implement constructors whose member initializer list can throw an exception.
*                                                 All the members' initialization declarations are placed after try expression. Lambdas cannot have a try-block implementation.
*                                                 The constexpr functions values can be evaluated at compile time.
*/

//try-block function
void TryCatchFunction(unique_ptr<int>& smartPtr)
try
{
    if(!smartPtr)
    {
        smartPtr = make_unique<int>(1054);
    }
    cout<<*smartPtr<<endl;
}
catch(const std::exception& exc)
{
    cout<<exc.what()<<endl;
}

class TryCatchCtor
{
    public:
        TryCatchCtor(const int value) try : spi{make_shared<int>()}
                                        {
                                            *spi = value;
                                            cout<<*spi<<endl;
                                        }
                                        catch(const std::exception& e)
                                        {
                                            cout<<e.what()<<endl;
                                        }
    
    private:
        shared_ptr<int> spi;
};

//constexpr function
constexpr int ConstExprFunc(int x)
{
    int res{};
    if(x > 0)
        res = x;
        
    return res;
}

class ConstExprClass
{
  public:
    constexpr ConstExprClass(const double d):m_d{d}{};
    double GetD() const {return m_d;};
  
  private:
    double m_d;
};

/*
* 7. Return type deduction for functions: It's deduced by the compiler from the type of the expression used in the return statement. 
*                                         If there are multiple return statements, they must all deduce to the same type.
*                                         If the return statement uses a braced-init-list, deduction is not allowed.
*/
//return type deduction for function
auto ReturnTypeDeducFunc()
{
    double pi=3.1415, e = 2.7182;
    return pi*e;
}

/*
* 8. Aggregate classes with default initializers for non-static members: An aggregate class, typically struct or union are used, is used to pack elements of distinct types together.
*                                                                        Thus, it does not have a constructor, neither default, inherited, explicit or user provided.
*                                                                        Since c++14 it can have a default member initializer for non static members.
*/
class AggregateClass
{
    public:
    int i;
    char c;
    double d;
    string str;
};
