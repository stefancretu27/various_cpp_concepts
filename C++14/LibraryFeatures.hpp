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

/*
* Library features in c++14:
* 1. make_unique<T>: Allows for dinamically allocate object of type T that is wrapped by the unique_ptr, which uniquely points to it. 
*                    Before c++14, a unique_ptr was initialized using the new function as argument of the unique_ptr ctor, either upon declaration or later, 
*                    as the ctor requires raw pointer argument.
*
* 2. <shared_mutex> library: In addition to the <mutex> library introduced in c++11, which provides mutex, timed_mutex, recursive_mutex and recursive_timed_mutex
*                            mutex types, as well ass lock_guard and unique_lock, this new library provides 2 extra functionalities in c++14:"<<endl;
*    
*                            2.1 shared_timed_mutex: Like other types of mutexes, it is a synchronization mechansim which prevent multiple threads from 
*                                                    simultaneously accessing a shared data. 
*                                                    Unlike those, it may not offer exclusive access, as the mutex ownership can be shared among several threads. 
*                                                    It can be done using lock_shared() and try_lock_shared() methods. 
*                                                    However, it comes with 2 levels of access, also allowing for exclusive ownership, like above listed mutexes, 
*                                                    using lock() and try_lock(). 
*                                                    The shared ownership is useful when multiple reader threads can access the critical zone without causing 
*                                                    data races, but when writing/modifying, the threads must be synchronized. Furthermore, it can be locked in a timely manner,
*                                                    like timed_mutex, using try_lock_for() and try_lock_until(). 
*                                                    If a thread aims to lock the shared_timed_mutex in a shared fashion when the max number of shared owners is achieved, the
*                                                    lock function will block till the number of shared owners decreases. The max number of shared owners is guaranteed to be at least 10000.
*                                                    When a shared_timed_mutex is acquired in exclusive fashion (lock_guard, unique_lock, lock) no other thread can lock it. However, if a
*                                                    shared_timed_mutex is locked in a shared manner, it can only be locked by other threads only in a shared fashion (shared_lock, lock_shared())
*    
*                            2.2 shared_lock: Like wise lock_guard and unique_lock, it is a mutex ownership wrapper class allowing for owning a mutex in RAII style. 
*                                             Unlike those, this wrapper owns the mutex, by default, only in a shared manner.
*                                             Likewise unique_lock, it offers interface for locking the mutex in a timed way: try_lock_for(), try_lock_until(), besides lock() and unlock().
*                                             However, shared_lock cannot be used with condition_variable, as the latter requires unique_lock. Thus, condition_variable_any can be used 
*                                             to be notified when using a mutex locked in shared mode.
*
*
* 5. std::quoted: Implemented in the <iomanip> header, it allows extraction and insertion of quoted strings, such as the ones in XML files.
*                 It adds quotes to the input string. The unquotting is done using a stringstream, where the quoted string is written to,
*                 then the content is written out to a quoted call, taking as input another string to store the result.
*                 Returns an object of unspecified type, hence the need to write the result as a stream of character to stringstream
*/

void joinThread(thread& t)
{
    if(t.joinable())
    {
        t.join();
    }
}

/*
* 3. std::integer_sequence<T, T ... Ints>: Implemented in the <utility> header - where pair, move and forward are also implemented - it represents a compile time sequence of integers.
*                                          The template type parameter T represents the type of integer (long, short, unsigned, signed, int32_t, int_fast16_t etc),
*                                          whereas the template non-type parameter is a pack representing the sequence of numbers, which can be deduced when used as
*                                          argument to template functions. It is useful for indexes, as it is known at compile time. It cannot be iterated over using loops.
*    
*                                         A helper alias template std::index_sequence is defined for the common case where T is std::size_t.
*                                         Helper alias templates std::make_integer_sequence and std::make_index_sequence are defined to simplify creation of std::integer_sequence and 
*                                         std::index_sequence types, respectively, with 0, 1, 2, ..., N-1 as Ints. An integer_sequence cannot be indexed at each element for iteration 
*                                         or precise element access. Instead, there can be built a std::array based on it, which can then be manipulated.
*/

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
/*
* 4. std::exchange<T, U>(T&& t, U&& u): Also implemented in the <utility> header, it is used to replace the value of the first argument with the value of the"<<endl;
*                                      second argument, returning the old value of the first argument. 
*                                        {auto temp = t; t=u; return temp;}
*                                      The second operand remains unchanged, as its content is not removed.
*                                      Used for implementing move c-tor and move assignment operator.
*/
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
