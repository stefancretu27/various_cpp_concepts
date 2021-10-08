
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
	cout<<"     1. The class thread is implemented in the header <thread> and an instance of it represents a single thread of execution."<<endl;
	cout<<"     The thread instance starts to execute concurrently, immediately after the construction, the function provided as parameter to its c-tor."<<endl;
	cout<<"     The function's arguments are also provided as paramteres to thread c-tor, next to the function's name. Those parameters requiring to be "<<endl;
	cout<<"     passed by reference should be passed using ref(arg) such that the thread c-tor is informed it has to deal with pass by ref and not pass by value."<<endl;
	cout<<"     The entry point function can communicate its return value to the main thread in 2 ways: "<<endl;
    cout<<"         1. using std::promise "<<endl;
    cout<<"         2. by modifying shared variables, requiring synchronization mechanisms such as std::atomic, std::mutex, std::condition_variable."<<endl<<endl;

    cout<<"     2. A thread instance does not represent a thread in execution. As mentioned above, the thread commences execution when thread c-tor is called."<<endl;
	cout<<"     It is told that the thread instance does not represent a thread. Such situations can be achieved in 3 ways:"<<endl;
	cout<<"         1. via default construction. Usually seen when a thread is a class member and it begins execution in the class c-tor or in other method."<<endl;
	cout<<"         2. when the thread is joined. The method joinable() checks if the thread can be joined. After joining, the thread terminates and as a result"<<endl;
	cout<<"     the thread instance has no longer a thread bound to it."<<endl;
    cout<<"         3. after the thread is moved from that instance and assigned to another instance."<<endl;
    cout<<"         4. upon a call to detach(). Detaching means the thread of execution is separated from the thread object. Thus it cannot be joined as it is not joinable."<<endl<<endl;
    
    cout<<"     3. When a thread is detached, it is separated from the caller thread and there is no need to be joined, as it becomes non joinable. When the detached thread finishes execution"<<endl;
    cout<<"     it destroys all allocated resources, including those received as arguments. Hence, when detaching a thread, all arguments must be passed by value, thus avoiding possible issues like"<<endl;
    cout<<"		double free or accessing a variable that has been deallocated by the other thread. Here is a scenario where passing shared_ptr by value and, in general, where the shared ownership makes sense."<<endl;
    
    cout<<"     4. When the thread of execution finishes its execution, it deallocates all resources, including those received as arguments. Here is where it "<<endl;
    cout<<"     might get into race conditions with the main thread, as the main thread can deallocate resources before the detached thread terminates, or viceversa."<<endl<<endl;

    

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

	//-------------------------------------------------------------------------------------------------------------------------
	//invoke functions entailing discussions on other multithreaded-related topics

	cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
    mutexInsights();
    cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
	atomicInsights();
	cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
	promise_futureInsights();

    return 0;
}
