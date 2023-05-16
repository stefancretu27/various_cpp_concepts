#include <iostream>
#include <memory>
using namespace std;

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

//return type deduction for function
auto ReturnTypeDeducFunc()
{
    double pi=3.1415, e = 2.7182;
    return pi*e;
}

class AggregateClass
{
    public:
    int i;
    char c;
    double d;
    string str;
};
