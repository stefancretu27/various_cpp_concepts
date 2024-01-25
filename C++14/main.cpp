#include "LibraryFeatures.hpp"
#include "LanguageFeatures.hpp"


int main()
{
    /*
    * C++14 new library features
    */
    
    // 1. make_unique<T>
    cout<<"------1. make_unique<T>------"<<endl;
    
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
    
    /*
    * C++14 new language features
    */
    cout<<"-----C++14 new language features------"<<endl;
    cout<<"------a. Variable templates------"<<endl;

    VariableTemplate::SetStaticTemplVariable(8);
    cout<<VariableTemplate::GetStaticTemplVariable<int>()<<endl;
    
    VariableTemplate::SetStaticTemplVariable(2.7182);
    cout<<VariableTemplate::GetStaticTemplVariable<double>()<<" "<<VariableTemplate::GetStaticTemplVariable<int>()<<endl;
    
    //instantiation and assignment
    myGenericVariable<int> = 12;
    myGenericVariable<char> = 'c';
    //explicit instantiation of the template
    myGenericVariable<float>;
    //assignment to that instantiation
    myGenericVariable<float> = 3.1415f;
    //reuse an existing instantiation, overwritting its value
    myGenericVariable<int> = -98;
    
    cout<<myGenericVariable<int><<" "<<myGenericVariable<float><<" "<<myGenericVariable<char><<endl;
    
    anotherGenericVar<double> = {4.4};
    int anotherGenericVar{12};

    
    cout<<"------b. Generic lambda------"<<endl;
    
    unique_ptr<int> uPtrInt{make_unique<int>(8)};
    auto lambda = [](const auto& data)
    {
        cout<<*data<<endl;  
    };
    
    lambda(uPtrInt);
    cout<<*uPtrInt<<endl;
    
    cout<<"------c. Capture-init for lambdas------"<<endl;
    
    string s;
    
    auto InitCaptureClosure = [ s = "initialized in capture list" ]() mutable
    {
        cout<<"inside lambda "<<s<<endl;
    };
    
    InitCaptureClosure();
    cout<<"outside lambda "<<s<<endl;
    
    int* pi;
    shared_ptr<int> sPtrInt;
    
    auto InitCaptureClosureDynamic = [pi = new int{2}, sPtrInt = make_shared<int>()] 
    {
        cout<<"inside lambda: "<<*pi<<endl;
        if(sPtrInt)
        {
            cout<<++(*sPtrInt)<<" use count "<<sPtrInt.use_count()<<endl;
        }
    };
    
    InitCaptureClosureDynamic();
    if(pi)
        cout<<"outside lambda "<<*pi<<endl;
        
    if(sPtrInt)
    {
        cout<<++(*sPtrInt)<<endl;
    }
    
    cout<<"------d. new/delete elision------"<<endl;

    cout<<"------e. Relaxed restrictions on constexpr functions------"<<endl;
    
    //function try-block example
    unique_ptr<int> smartUPtr;
    TryCatchFunction(smartUPtr);
    TryCatchCtor inst{100};
    
    constexpr int cei = ConstExprFunc(3);
    constexpr int cei2 = ConstExprFunc(-3);
    cout<<cei<<" "<<cei2<<endl;
    
    //works only if x is const or constexpr
    const int x{7};
    constexpr int cei3 = ConstExprFunc(x);
                                        
    ConstExprClass ceObj{5};
    cout<<ceObj.GetD()<<" "<<cei3<<endl;
    
    cout<<"------f. Binary literals------"<<endl;

    int binaryLiteral = 0b1101;
    int digitSeparators = 10'938'756;
    double floatDigitSeparators = 3.141'59;
    cout<<binaryLiteral<<" "<<digitSeparators<<" "<<floatDigitSeparators<<endl;
    
    cout<<"------g. Return type deduction for functions------"<<endl;

    cout<<ReturnTypeDeducFunc()<<endl;
    
    cout<<"------h. aggregate classes with default non-static member initializers ------"<<endl;

    AggregateClass aggr{22, 'x', 3.1415, "aggr"};
    cout<<aggr.i<<" "<<aggr.str<<endl;
    
    return 0;
}
