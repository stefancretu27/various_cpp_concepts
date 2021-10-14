#include "headers.hpp"

using namespace std;

void mutexInsights()
{
	cout<<endl<<"Insights on std mutex"<<endl;
    cout<<"     1. mutex is a template class implemented in header <mutex>"<<endl;
    cout<<"     A mutex instance is NOT MOVABLE or COPYABLE => mutex class does not implement any copy or move semantics."<<endl;

	cout<<"     2. Mutex is a synchronization primitive that ensures exclusive access to a shared recource amongst multiple threads attempting to access it."<<endl;
	cout<<"     A thread attempting to own a mutex must not own already the same mutex, otherwise it leads to undefined behavior, possibly deadlock."<<endl<<endl;
	
	cout<<"     3. A mutex is owned when it is successfully acquired using one of the following:"<<endl;
	cout<<"         1. mutex.lock() - acquire mutex. If already acquired by another thread, this call blocks here until the mutex is released."<<endl;
    cout<<"         2. mutex.try_lock() - tries to lock the mutex and returns immediately. On success returns true, false is returned otherwise."<<endl<<endl;
    cout<<"      When lock or try_lock are used to acquire a mutex, it must be released explicitly using mutex.unlock(). Attempting to unlock a mutex"<<endl;
    cout<<"     that is not locked, leads to undefined behavior."<<endl<<endl;

    cout<<"     4. lock_guard<std::mutex> lg(mutex) - it is a template class which wraps over mutex, allowing for acquiring a mutex in RAII fashion, "<<endl;
    cout<<"     within the scope where the lock_guard is defined. When lock_guard is destroyed, at the end of the scope where it is defined,"<<endl;
    cout<<"     its d-tor automatically calls unlock(). A lock_guard is NOT COPYABLE or MOVABLE => both copy and move semantics are deleted."<<endl<<endl;

    cout<<"     4. unique_lock<std::mutex> lg(mutex) - it is a template class which wraps over mutex, allowing for deferred locking, recursive locking "<<endl;
    cout<<"     move of locking ownership, time-constrained attempt of locking and use in conjuction with condition variables."<<endl;
    cout<<"     A unique_lock is NOT COPYABLE, but it is MOVABLE => only its copy semantics is deleted."<<endl;
    cout<<"      - defer_lock option allows construction of an unique_lock that wraps a given mutex, but does not lock it. An explicit call to lock"<<endl;
    cout<<"     is required, which allowes for acquiring of multiple locks using deadlock avoidance algorithm. "<<endl<<endl;
    
     auto entryPointRawPtr{  [](shared_ptr<HelperClass>& hc, mutex& hc_mutex)
                            {
                                lock_guard<mutex> guard(hc_mutex);
                                if(hc)
                                {
                                    
                                    cout<<"use_count: "<<hc.use_count()<<" Line: "<<__LINE__<<endl;
                                }
                                else
                                {
                                    cout<<" null ptr passed as arg"<<endl;
                                }
                            }
                        };

    shared_ptr<HelperClass> sphc{make_shared<HelperClass>()};
    mutex sphc_mutex;
    thread threadInst = thread(entryPointRawPtr, ref(sphc), ref(sphc_mutex));

    if(sphc_mutex.try_lock())
    {
        cout<<__LINE__<<" use_count: "<<sphc.use_count()<<endl;
        sphc_mutex.unlock();
    }
    
    joinThread(threadInst);
}
