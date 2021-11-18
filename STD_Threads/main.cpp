
#include "headers.hpp"
using namespace std;

void joinThread(std::thread& threadInst)
{
    //join the thread
    if(threadInst.joinable())
    {
        std::cout<<"thread "<<threadInst.get_id()<<" is joined"<<std::endl;
        threadInst.join();
    }
    else
    {
        std::cout<<"thread "<<threadInst.get_id()<<" is not joinable"<<std::endl;
    }
}

int main()
{
	cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
    cout<<endl<<"Insights on std thread"<<endl;
    cout<<"     1. Threads represent a mechanism that allows a program to execute multiple tasks concurrently. They allow for sharing data in between"<<endl;
    cout<<"     them with ease, as they share the same global memory and heap memory with the main program. "<<endl;
    cout<<"     The main program constitues a thread itself which can be referred to as main thread or default thread. Each thread executes a target function, "<<endl;
    cout<<"     which in its turn can call other (sub)functions. Sometimes, the target function is referred to as thread's entry point."<<endl;
    cout<<"     The target functions can use shared data (global or static variables) which are accessible within each thread. That said, when a code "<<endl;
    cout<<"     segment can be accessed by multiple threads it is referred to as CRITICAL SECTION, and the concurrent access to it requires use of"<<endl;
    cout<<"     synchronization mechanisms. These mechanisms are useful to prevent RACE CONDITIONS, which occur when at least to threads attempt to access"<<endl;
    cout<<"     the same data simultaneously."<<endl;

	cout<<"     2. The class thread is implemented in the header <thread> and an instance of it represents a single thread of execution."<<endl;
	cout<<"     The thread instance starts to execute concurrently, immediately after the construction, the function provided as parameter to its c-tor."<<endl;
	cout<<"     The function's arguments are also provided as paramteres to thread c-tor, next to the function's name. Those parameters requiring to be "<<endl;
	cout<<"     passed by reference should be passed using ref(arg) such that the thread c-tor is informed it has to deal with pass by ref and not by value."<<endl;
	cout<<"     The entry point function can communicate its return value to the main thread in 2 ways: "<<endl;
    cout<<"         1. using std::promise "<<endl;
    cout<<"         2. by modifying shared variables, requiring synchronization mechanisms such as std::atomic, std::mutex, std::condition_variable."<<endl<<endl;

    cout<<"     3. A thread instance does not represent a thread in execution. As mentioned above, the thread commences execution when thread c-tor is called."<<endl;
	cout<<"     It is told that the thread instance does not represent a thread. Such situations can be achieved in 3 ways:"<<endl;
	cout<<"         1. via default construction. Usually seen when a thread is a class member and it begins execution in the class c-tor or in other method."<<endl;
	cout<<"         2. when the thread is joined. The method joinable() checks if the thread can be joined. After joining, the thread terminates and as "<<endl;
	cout<<"     a result the thread instance has no longer a thread bound to it."<<endl;
    cout<<"         3. after the thread is moved from that instance and assigned to another instance."<<endl;
    cout<<"         4. upon a call to detach(). Detaching means the thread of execution is separated from the thread object. Thus, it cannot be joined"<<endl;
    cout<<"     as it is not joinable."<<endl<<endl;

    cout<<"     4. When a thread is detached, it is separated from the caller thread and becomes non joinable. When the detached thread finishes its "<<endl;
    cout<<"     execution, it destroys all allocated resources, including those received as arguments. Hence, when detaching a thread, all arguments "<<endl;
    cout<<"		must be passed by value, thus avoiding possible issues like double free or accessing a variable that has been deallocated by the other"<<endl;
    cout<<"     thread. Here is a scenario where passing shared_ptr by value and, in general, where the shared ownership makes sense."<<endl<<endl; 
    
    cout<<"     5. A thread object is NOT COPYABLE, as its copy semantics is deleted, thus there is impossible to have 2 or more objects representing "<<endl;
    cout<<"     the same execution thread, as to each thread is assigned an unique id. Nonetheless, a thread object is movable."<<endl<<endl;

    cout<<"     6. Attributes specific to each thread are:"<<endl;
    cout<<"         - thread ID = useful to identify on which thread a certain action occurs: join, detach (both in pthread and <thread>), kill, cancel"<<endl;
    cout<<"         - signal mask"<<endl;
    cout<<"         - stack frame"<<endl;
    cout<<"         - thread specific data = "<<endl;
    cout<<"         - real time scheduling policy an priority = "<<endl;
    cout<<"         - CPU affinity = "<<endl;
    cout<<"         - capabilities = "<<endl;
    cout<<"         - errno variable, floating point environment and alternate signal stack"<<endl;

    cout<<"     7. Attributes that are shared amongst thread are:"<<endl;
    cout<<"         - porgram's stack text (contains all instructions to be executed by that program)"<<endl;
    cout<<"         - program's stack heap area (all dinamycally allocated memory)"<<endl;
    cout<<"         - program's stack .data, .rodata, .bss areas (globals, const globals and statics)"<<endl;
    cout<<"         - process ID, parent process ID, proces group ID"<<endl;
    cout<<"         - file descriptors table (all open files of the process)"<<endl;
    cout<<"         - controlling terminal "<<endl;
    cout<<"         - file system related information: current working directory and root directory"<<endl;
    cout<<"         - resources limits, CPU time consumed and resources consumed"<<endl;

    cout<<"     8. Reentrant functions achieve thread safety without using synchronization mechanisms, that hamper the overall performance, by avoiding"<<endl;
    cout<<"     global, static and shared variables with other threads. Instead, such a function maintains all its data locally and any information that"<<endl;
    cout<<"     must be returned to the caller is stored in buffers allocated by the caller. In order to avoid critical sections when dealing with such"<<endl;
    cout<<"     buffers, each thread can get its own buffers to write the results in."<<endl<<endl;

    cout<<"     9. thread_local is a duration specifier, likewise auto, static and dynamic, which denotes that a variable storage is set aside when the"<<endl;
    cout<<"     thread starts and is deallocated when the thread ends, in an automatic fashion. The specifier is placed before the bariable's type and"<<endl;
    cout<<"     the variable envisaged for being used in the thread's local storage should be declared outside that thread, such as in the main thread."<<endl;
    cout<<"     That said, once declared at a 'level above', each thread can access the variable, in order only to get a copy of it. In addition, "<<endl;
    cout<<"     thread_local duration specifier can be used in conjunction with static or extern linkage specifiers, in order to set the linkage."<<endl;
    cout<<"     There is no point in making a thread_local variable atomic, as each thread gets its own copy so no race condition can occur on it."<<endl;
    cout<<"     thread_local variables cannot be captured in lambdas as their are seen as static variables, which cannot be captured as well."<<endl<<endl;

    //create 2 thread instances using the default thread c-tor
    thread defaultCreatedInst{};
    thread threadInst;

    //create top level function, also referred to as the entry point
    function<void(shared_ptr<int>&)> entryPointByRef{   [](shared_ptr<int>& spi)
                                                        {
															cout<<"Shared ptr passed by ref to this thread entry point, as the thread shall be joined"<<endl;
                                                            if(spi)
                                                            {
                                                                cout<<"value: "<<*spi<<" use count: "<<spi.use_count()<<endl;
                                                            }
                                                            else
                                                            {
                                                                cout<<" null ptr passed as arg"<<endl;
                                                            }
                                                        }
                                                    };

    auto entryPointByValue{ [](shared_ptr<int> spi)
                            {
								cout<<"Shared ptr passed by value to this thread entry point, as the thread shall be detached"<<endl;
                                if(spi)
                                {
                                    cout<<"value: "<<*spi<<" use count: "<<spi.use_count()<<endl;
                                }
                                else
                                {
                                    cout<<" null ptr passed as arg"<<endl;
                                }
                            }
                          };
    //create argument for the entry point
    shared_ptr<int> spi{make_shared<int>(8)}, spi2{make_shared<int>(9)};

    //assign a thread of execution to the existing thread instance
    threadInst = thread(entryPointByRef, ref(spi));
    joinThread(threadInst);
    
    //assign new thread, then detach it => pass shared ptr by value
    threadInst = thread(entryPointByValue, spi2);
    threadInst.detach();

    //declare thread_local variable and initialize it outisde the thread function
    thread_local int_least16_t i16{};

    auto threadLocalFunction{[]()
                                {
                                    //each function gets its own copy, so no race condition
                                    ++i16;
                                    //each function prints 1
                                    cout<<this_thread::get_id<<" "<<i16<<endl;
                                }

                            };

    auto threadLocalFunctionByRef{[](int_least16_t& iLeast16)
                                {
                                    //each function gets its own copy, so no race condition
                                    ++iLeast16;
                                    //each function prints 1
                                    cout<<this_thread::get_id<<" "<<i16<<endl;
                                }

                            };
    
    thread thLocal1, thLocal2, thLocal3;
    thLocal1 = thread(threadLocalFunction);
    thLocal2 = thread(threadLocalFunction);
    thLocal3 = thread(threadLocalFunctionByRef, ref(i16));

    cout<<"access thread local variable outside the threads: "<<--i16<<endl;

    joinThread(thLocal1);
    joinThread(thLocal2);
    joinThread(thLocal3);

	//-------------------------------------------------------------------------------------------------------------------------
	//invoke functions entailing discussions on other multithreaded-related topics


    cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
	atomicInsights();
	cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
	promise_futureInsights();
    cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
    mutexInsights();
    cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
    conditionVariableInsights();

    return 0;
}
