
#include "headers.hpp"
using namespace std;

/*
* <thread> library introducced in C++11:
*	1. Threads represent a mechanism that allows a program to execute multiple tasks concurrently, being the smallest sequence of programmed instructions that can be managed independently 
*	   by a scheduler, which is typically a part of the OS. They allow for sharing data in between them with ease, as they share the same global memory and heap memory with the main program. 
*	   The main program constitues a thread itself which can be referred to as main thread or default thread. Each thread executes 
*	   a target function, which in its turn can call other (sub)functions. Sometimes, the target function is referred to as thread's entry point.
*	   The target functions can use shared data (global or static variables) which are accessible within each thread. That said, when a code segment can be accessed by multiple threads
*	   it is referred to as CRITICAL SECTION, and the concurrent access to it requires use of synchronization mechanisms. These mechanisms are useful to prevent RACE CONDITIONS, which occur 
*	   when at least to threads attempt to access the same data simultaneously.
*
*	2. The class thread is implemented in the header <thread> and an instance of it represents a single thread of execution. The thread instance starts to execute concurrently, 
*	     immediately after the construction, the function provided as parameter to its c-tor. The function's arguments are provided as parameters to thread c-tor, by copy or moved. 
*	     If passing by reference is desired, then for each argument there must be used ref/cref wrappers.
*	     The entry point function can communicate its return value to the main thread in 2 ways: 
*		 2.1. using std::promise
*		 2.2. by modifying shared variables, requiring synchronization mechanisms such as std::atomic, std::mutex, std::condition_variable.
*
*	3. A thread instance does not represent a thread in execution. As mentioned above, the thread commences execution when thread c-tor is called.
*	    It is told that the thread instance does not represent a thread. Such situations can be achieved in 3 ways:
*	        3.1. via default construction. Usually seen when a thread is a class member and it begins execution in the class c-tor or in other method.
*		3.2. when the thread is joined. The method joinable() checks if the thread can be joined. After joining, the thread terminates and, as a result, the thread instance has no longer a 
*			thread bound to it.
*		3.3. after the thread is moved from that instance and assigned to another instance.
*		3.4. upon a call to detach(). Detaching means the thread of execution is separated from the thread object, being handled by the OS. Thus, it cannot be joined as it is not joinable.
*
*	4. When a thread is detached, it is separated from the caller thread and becomes non joinable. When the detached thread finishes its execution, it destroys all allocated resources, 
*           including those received as arguments. Hence, when detaching a thread, all arguments must be passed by value, thus avoiding possible issues like double free or accessing a variable 
*	    that has been deallocated by the other thread. Here is a scenario where passing shared_ptr by value and, in general, where the shared ownership makes sense.
*
*	5. No two std::thread objects may represent the same thread of execution; std::thread is not CopyConstructible or CopyAssignable, although it is MoveConstructible and MoveAssignable.
*
*	6. Attributes specific to each thread are:
*		6.1 thread ID = useful to identify on which thread a certain action occurs: join, detach (both in pthread and <thread>), kill, cancel"<<endl;
* 		6.2 signal mask
*		6.3 stack frame
*		6.4 thread specific data: keyword thread_local - thread storage duration. 
*		6.5 real time scheduling policy an priority 
*		6.6 CPU affinity 
*		6.7 capabilities 
*		6.8 errno variable, floating point environment and alternate signal stack
*
*	7. Attributes that are shared amongst thread are:
*		7.1 porgram's stack text (contains all instructions to be executed by that program)
*		7.2 program's stack heap area (all dinamycally allocated memory)
* 		7.3 program's stack .data, .rodata, .bss areas (globals, const globals and statics)
*		7.4 process ID, parent process ID, proces group ID
* 		7.5 file descriptors table (all open files of the process)
*		7.6 controlling terminal 
*		7.8 file system related information: current working directory and root directory
*		7.9 resources limits, CPU time consumed and resources consumed
*
*	8. Reentrant functions achieve thread safety without using synchronization mechanisms, that hamper the overall performance, by avoiding global, static and shared variables with other threads. 
*	   Instead, such a function maintains all its data locally and any information that must be returned to the caller is stored in buffers allocated by the caller. In order to avoid critical sections 
*	   when dealing with such buffers, each thread can get its own buffers to write the results in.
*
*	9. thread_local is a duration specifier, likewise auto, static and dynamic, which denotes that a variable storage is set aside when the thread starts and is deallocated when the thread ends, 
*	   in an automatic fashion. The specifier is placed before the bariable's type and the variable envisaged for being used in the thread's local storage should be declared outside that thread, 
*	   such as in the main thread. That said, once declared at a 'level above', each thread can access the variable, in order only to get a copy of it. In addition, thread_local duration specifier 
*	   can be used in conjunction with static or extern linkage specifiers, in order to set the linkage.There is no point in making a thread_local variable atomic, as each thread gets its own copy 
*	   so no race condition can occur on it. thread_local variables cannot be captured in lambdas as their are seen as static variables, which cannot be captured as well.
*
*	10. Header functions:
*		10.1 std::this_thread::yield() - Provides a hint to the implementation to reschedule the execution of threads, allowing other threads to run. 
*                  The exact behavior of this function depends on the implementation, in particular on the mechanics of the OS scheduler in use and the state of the 
*                  system. For example, a first-in-first-out realtime scheduler (SCHED_FIFO in Linux) would suspend the current thread and put it on the back of the 
*                  queue of the same-priority threads that are ready to run.
*		10.2 std::this_thread::get_id() returns the unique ID of the current thread, as std::thread::id data type
*		10.3 std::this_thread::sleep_for(sleep_duration eg:2000ms) - Blocks the execution of the current thread for at least the specified sleep_duration. It 
*                   may block  for longer due to scheduling or resource delays. The standard recommends that a steady clock is used to measure the duration.
*               10.4 std::this_thread::sleep_until(sleep_time ) - Blocks the execution of the current thread until the specified sleep_timepoint is reached.
*/

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
    cout<<endl<<"Insights on std thread"<<endl;

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
