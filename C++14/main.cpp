#include "LibraryFeatures.hpp"

int main()
{
    /*
    * C++14 new library features
    */
    
    // 1. make_unique<T>
    cout<<"------1. make_unique<T>------"<<endl;
    
    cout<<"     Allows for dinamically allocate object of type T that is wrapped by the unique_ptr, which"<<endl;
    cout<<"     uniquely points to it. Before c++14, a unique_ptr was initialized using the new function"<<endl;
    cout<<"     as argument of the unique_ptr ctor, either upon declaration or later, as the ctor requires"<<endl;
    cout<<"     raw pointer argument"<<endl;
    
    unique_ptr<int> uPtrInt{make_unique<int>()};
    unique_ptr<float> uPtrFloat;
    uPtrFloat = make_unique<float>(3.14159);
    
    cout<<*uPtrInt<<" "<<*uPtrFloat<<endl;
    
    //error:use of deleted copy c-tor
    //unique_ptr<float> copy = uPtrFloat;
    
    // before c++14
    unique_ptr<double> uPtrDouble;
    uPtrDouble = unique_ptr<double>(new double{2.7182});
    //uPtrDouble = unique_ptr<double>(-273.1);   requires passing the raw pointer 
    unique_ptr<string> uPtrString{new string{"string"}};
    
    cout<<*uPtrDouble<<" "<<*uPtrString<<endl;
    
    // 2. <shared_mutex> library
    cout<<"------2. <shared_mutex> library------"<<endl;
    
    cout<<"     In addition to <mutex> library introduced in c++11, which provides mutex, timed_mutex, recurisve_mutex and recursive_timed_mutex"<<endl;
    cout<<"     mutex types, as well ass lock_guard and unique_lock, this new library provides 2 extra functionalities in c++14:"<<endl;
    
    cout<<"     2.1 a new mutex type: shared_timed_mutex. Like other types of mutexes, it is a synchronization mechansim which"<<endl;
    cout<<"     prevent multiple threads from simultaneously access a shared data. Unlike those, it may not offer exclusive access,"<<endl;
    cout<<"     as the mutex ownership can be shared among several threads.It can be done using lock_shared() and try_lock_shared() methods"<<endl;
    cout<<"     However, it comes with 2 levels of access, also allowing for exclusive ownership, like above listed mutexes, using lock()"<<endl;
    cout<<"     and try_lock(). The shared ownership is useful when multiple reader threads can access the critical zone without causing "<<endl;
    cout<<"     data races, but when writing/modifying threads must be synchronized. Furthermore, it can be locked in a timely manner,"<<endl;
    cout<<"     like timed_mutex, using try_lock_for() and try_lock_until(). In addition, the shared_timed_mutex offers the possibility"<<endl;
    cout<<"     to be locked in a timely manner, whereas sharing ownership amongst some threads: try_lock_shared_for() try_lock_shared_until()."<<endl;
    cout<<"     If a thread aims to lock the shared_timed_mutex in a shared fashion when the max number of shared owners is achieved, the"<<endl;
    cout<<"     lock function will block till the number of shared owners decreases. The max number of shared owners is guaranteed to be at least 10000."<<endl;
    cout<<"     When a shared_timed_mutex is acquired in exclusive fashion (lock_guard, unique_lock, lock) no other thread can lock it. However, if a"<<endl;
    cout<<"     shared_timed_mutex is locked in a shared manner, it can only be locked by other threads only in a shared fashion (shared_lock, lock_shared())"<<endl;
    
    cout<<"      2.2 a new locking mechanism: shared_lock. Like wise lock_guard and unique_lock, it is a mutex ownership wrapper class"<<endl;
    cout<<"      allowing for owning a mutex in RAII style. Unlike those, this wrapper owns the mutex, by default, only in a shared manner."<<endl;
    cout<<"      Likewise unique_lock, it offers interface for locking the mutex in a timed way: try_lock_for(), try_lock_until(), besides lock()"<<endl;
    cout<<"      and unlock()."<<endl;
    cout<<"      However, shared_lock cannot be used with condition_variable, as the latter requires unique_lock. That said, condition_variable_any"<<endl;
    cout<<"      can be used to be notified when using a mutex locked in shared mode."<<endl;
    
    shared_timed_mutex sharedTimedMtx;
    
    auto dataModifier = [&uPtrInt, &sharedTimedMtx]
                        {
                            sharedTimedMtx.lock();
                            cout<<"Modifying the critical section "<<*uPtrInt<<endl;
                            ++(*uPtrInt);
                            cout<<"Modified the critical section "<<*uPtrInt<<endl;
                            sharedTimedMtx.unlock();
                        };
    
    auto dataReader1 = [&uPtrInt, &sharedTimedMtx]
                        {
                            //allow the writer to go first
                            this_thread::sleep_for(chrono::milliseconds(20));
                            
                            shared_lock<shared_timed_mutex> sharedLck{sharedTimedMtx};
                            cout<<"Shared_timed_mutex shared_lock. Reading the critical section "<<*uPtrInt<<endl;
                        };
    
    auto dataReader2 = [&uPtrInt, &sharedTimedMtx]
                        {
                            this_thread::sleep_for(chrono::milliseconds(20));
                            
                            if(sharedTimedMtx.try_lock_shared_until(chrono::steady_clock::now() + chrono::seconds(2)))
                            {
                                cout<<"Shared_timed_mutex try_lock_shared_until. Reading the critical section "<<*uPtrInt<<endl;
                                sharedTimedMtx.unlock_shared();
                            }
                        };
    
    thread t1{dataModifier}, t2{dataReader1}, t3{dataReader2};
    
    joinThread(t1);
    joinThread(t2);
    joinThread(t3);
    
    condition_variable_any condVarAny;
    bool isDataUpdated{false};
    
    auto producer = [&sharedTimedMtx, &condVarAny, &isDataUpdated, &uPtrString]()
                    {
                        {
                            lock_guard<shared_timed_mutex> lckGuard{sharedTimedMtx};
                            cout<<" Producer thread using lock guard with shared_timed_mutex and condition_variable_any "<<endl;
                            *uPtrString = " modified string using shared_timed_mutex and lock_guard";
                            isDataUpdated = true;
                        }
                        condVarAny.notify_all();
                    };
                    
    auto consumer = [&sharedTimedMtx, &condVarAny, &isDataUpdated, &uPtrString](int id)
                    {
                        {
                            shared_lock<shared_timed_mutex> sharedLck{sharedTimedMtx};
                            condVarAny.wait(sharedLck, [&isDataUpdated]{return isDataUpdated;});
                            cout<<" Consumer thread using shared_lock with shared_timed_mutex and condition_variable_any "<<id<<endl;
                            cout<<*uPtrString<<endl;
                        }
                    };
                    
    thread prodTh{producer}, consTh{consumer, 1}, consTh2{consumer, 2};
    
    joinThread(prodTh);
    joinThread(consTh);
    joinThread(consTh2);
    
    // 3. integer_sequence<T, T ... Ints>()
    cout<<"------3. integer_sequence<T, T ... Ints>()------"<<endl;
    
    cout<<"     Implemented in the <utility> header - where pair, move and forward are also implemented - it represents a compile time sequence of integers."<<endl;
    cout<<"     The template type parameter T represents the type of integer (long, short, unsigned, signed, int32_t, int_fast16_t etc)"<<endl;
    cout<<"     whereas the template non type parameter is a pack representing the sequence of numbers, which can be deduced when used as"<<endl;
    cout<<"     argument to template functions. It is useful for indexes, as it is known at compile time. It cannot be iterated over using loops."<<endl;
    
    cout<<"     A helper alias template std::index_sequence is defined for the common case where T is std::size_t."<<endl;
    cout<<"     Helper alias templates std::make_integer_sequence and std::make_index_sequence are defined to"<<endl;
    cout<<"     simplify creation of std::integer_sequence and std::index_sequence types, respectively, with 0, 1, 2, ..., N-1 as Ints"<<endl;
    cout<<"     An integer_sequence cannot be indexed at each element for iteration or precise element access. Instead, there can be built"<<endl;
    cout<<"     a std::array based on it, which can then be manipulated."<<endl;
    
    integer_sequence<unsigned, 0, 3, 4, 20, 17, 5, 4> unsignedSeq;
    print_sequence(unsignedSeq);

    //variadic template recursive function examples
    printData<unsigned>(0, 3, 4, 20, 17, 5, 4);
    printData("string", 'c', 2, -4, 2.0456, 1.01f);
    
    auto other_integer_sequence = make_integer_sequence<short, 6>();
    auto indexSeq = make_index_sequence<8>();

    //manipulate integer_sequence requires constructing a std::array from it
    cout<<GetElementAt(indexSeq, 5)<<" "<<GetElementAt(other_integer_sequence, 5)<<" "<<GetElementAt(unsignedSeq, 5)<<endl;
    PrintIntSequence(unsignedSeq);
    
    // 4. T exchange<T, U>(T t, U u)
    cout<<"------4. T exchange<T, U>(T& t, U&& u)------"<<endl;
    
    cout<<"     Also implemented in the <utility> header, it is used to replace the value of the first argument with the value of the"<<endl;
    cout<<"     second argument, returning the old value of the first argument. The second operand remains unchanged, as its content "<<endl;
    cout<<"     is not removed."<<endl;
    cout<<"     Used for implementing move c-tor and move assignment operator."<<endl;
    
    string str1{"first"}, str2{"second"};
    auto result = exchange(str1, str2);
    cout<<str1<<" "<<str2<<" "<<result<<endl;
    
    MoveSemantics inst1{result}, inst2;
    cout<<inst1.GetContent()<<endl;
    inst2 = move(inst1);
    cout<<" MoveSemantics "<<inst1.GetContent()<<" "<<inst2.GetContent()<<endl;
    
    CopyAndSwap obj1{"obj1"}, obj2{"obj2"}, aux;
    aux = obj1;
    cout<<aux.GetContent()<<" "<<obj1.GetContent()<<endl;
    aux = move(obj2);  //move the content to a copy
    cout<<aux.GetContent()<<" "<<obj2.GetContent()<<endl;
    
    // 5. quoted<class CharT>(const CharT* s, CharT delim = CharT('"'), CharT escape = CharT('\\')
    cout<<"------5. quoted<class CharT>(const CharT* s, CharT delim = CharT('\"'), CharT escape = CharT('\\')------"<<endl;
    
    cout<<"     Implemented in the <iomanip> header, it allows extraction and insertion of quoted strings, such as the ones in XML files."<<endl;
    cout<<"     It adds quotes tot he input string. The unquotting is done using a stringstream, where the quoted string is written to,"<<endl;
    cout<<"     then the content is written out to a quoted call, taking as input another string to store the result."<<endl;
    cout<<"     Return an object of unspecified type, hence the need to write the result as a stream of character to stringstream."<<endl;

    string unquotedString{"to be quoted string + \"inner quoted string\""};
    cout<<quoted(unquotedString)<<" - "<<unquotedString<<" - "<<quoted(unquotedString)<<endl;
    stringstream ss;
    ss << quoted(unquotedString);
    cout<<ss.str()<<endl;
    string unqouteResult;
    ss >> quoted(unqouteResult);
    cout<<unqouteResult<<endl;
    
    string quotedString{"aaa \"to be quoted string\" aaa "};
    cout<< quoted(quotedString)<<endl;

    return 0;
}
