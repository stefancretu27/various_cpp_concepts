#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <shared_mutex>
#include <condition_variable>
#include <utility>
#include <array>
#include <iomanip>
#include <sstream>
using namespace std;

void joinThread(thread& t)
{
    if(t.joinable())
    {
        t.join();
    }
}

template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    ((std::cout << ints << ' '), ...); //c++17 feature folding expr
    cout << endl;
}

template<class T>
void printData(T first)
{
    cout<<first<<endl;
}

template<class T, class ... Args>
void printData(T first, Args ... args)
{
    cout<<first<<" ";
    printData<Args ...>(args...);
}

template<class Int, Int ... Ints>
Int GetElementAt(integer_sequence<Int, Ints ...> intSeq, size_t index)
{
    
    return array<Int, intSeq.size()>{Ints ...}[index];
}

template<class Int, Int ... Ints>
void PrintIntSequence(integer_sequence<Int, Ints ...> intSeq)
{
    for (Int elem : array<Int, intSeq.size()>{Ints ...})
    {
        cout<<elem<<" ";
    }
    cout<<endl;
}

class MoveSemantics
{
    public:
    MoveSemantics() = default;
    explicit MoveSemantics(const string& str):content{str}{};
    
    MoveSemantics(MoveSemantics&& obj )
    {
        //leave obj content in a well defined, initial state, whereas its oled value
        //is returned and retained by local data
        content = exchange(obj.content, "");
    }
    
    MoveSemantics& operator=(MoveSemantics&& obj)
    {
        if(this==&obj)
        {
            return *this;
        }
        
        content = exchange(obj.content, "");
    }
    
    string GetContent() {return content;};
    
    private:
    string content;
};

//copy and swap uses one implementation for copy and move assignment operators
// a copy of the rhs operand is gotten when it is passed by value as argument => calls copy c-tor of the class
//the copy is swapped with the lhs, which is this, but the rhs operand is moved when passed as argument to swap
//the copy doesn't have to be left in a well defined state, when moving
class CopyAndSwap
{
    public:
    CopyAndSwap() = default;
    explicit CopyAndSwap(const string& str):content{str}{};
    
    //the rule of 4 members
    ~CopyAndSwap() = default;
    CopyAndSwap(CopyAndSwap& rhs):content{rhs.content}
    {
        cout<<" copy c-tor"<<endl;
    };
    CopyAndSwap(CopyAndSwap&& rhs):content{move(rhs.content)}
    {
        cout<<" move c-tor"<<endl;
    };
    
    //take a copy of the rhs object when passing as arg
    CopyAndSwap& operator=(CopyAndSwap obj )
    {
        cout<<" copy and swap assignment operator "<<obj.content<<endl;
        swap(content, obj.content);
        
        cout<<" copy and swap assignment operator "<<obj.content<<endl;
        return *this;
    }
    
    friend void swap(CopyAndSwap& lhs, CopyAndSwap& rhs)
    {
        using std::swap;
        
        swap(lhs.content, rhs.content);
    };
    
    string GetContent() {return content;};
    
    private:
    string content;
};
