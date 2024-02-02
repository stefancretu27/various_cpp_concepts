#include "headers.hpp"

#include <vector>
#include <chrono>

using namespace std;

/*
*	1. mutex is a template class implemented in header <mutex>, introduced in C++
*
* 	2. Mutex is a synchronization primitive that ensures exclusive access (atomic access) to a resource shared amongst multiple threads (critical section), thus avoiding data races.
*	When a thread acquires a mutex, it becomes the owner of that mutex and is the only responsible for unlocking it. A thread attempting to acquire a mutex must not own already the same mutex, 
*	otherwise it leads to undefined behavior, possibly deadlock. While a thread successfully acquired a mutex, the other threads attempting to lock the mutex are blocked until the mutex is unlocked.
*
*	3. Since a mutex provides exclusive access to a shared resource, when dealing with multiple shared resources, the approach is to have a mutex for each shared resource on which atomic access is required. 
*	Using a given mutex for more shared variables would be ambiguous and could lead to deadlocks or undefined behaviors. lock(mutex1, mutex2, ...mutexn) can be used to lock at least 2 mutexes.
*
*	4. A mutex instance is NOT MOVABLE or COPYABLE => mutex class does not implement any copy or move semantics. Also, in order to properly guard a critical section, all threads should use the same mutex 
*	when accessing it, such that each one can check if the mutex is locked. Thus, the mutex locking status is visible to all threads. If a mutex would have been copyable, a copy instance could be created on 
*	another thread. Thereafter, on the initial instance, but also on the copy instance lock operations would be possible. If the original mutex instance and the copy would be used in distinct threads, to 
*	protect the same critical section, then they would act as different mutexes, and locking both would be possible. Consequently, the access to the shared resource would not be exclusive.
*	Furthermore, if a mutex would be movable its locking status could be transferred from one thread to another, meaning the critical section it initially guarded remains unprotected.
*
*	5. A mutex is owned when it is successfully acquired using one of the following:
*		5.1. mutex.lock() - acquire mutex. If already acquired by another thread, this call blocks here until the mutex is released.
*		5.2. mutex.try_lock() - tries to lock the mutex and returns immediately. On success returns true, false is returned otherwise.
*	When lock or try_lock are used to acquire a mutex, it must be released explicitly using mutex.unlock(). Attempting to unlock a mutex that is not locked, leads to undefined behavior.
*
*	6. lock_guard<std::mutex> lg(mutex) - it is a template class which wraps over mutex, allowing for acquiring a mutex in RAII fashion, within the scope where the lock_guard is defined. 
*	When lock_guard is destroyed, at the end of the scope where it is defined, its d-tor automatically calls unlock(). A lock_guard is NOT COPYABLE or MOVABLE => both copy and move semantics are deleted.
*	Once a mutex is acquired using lock_guard, the lock_guard instance cannot be used to explicitly unlock and lock again the mutex, as the class does not provide lock(), unlock() interface. 
*	Locking is just performed upon lock_guard instantiation.
*
*	7. unique_lock<std::mutex> lg(mutex) - it is a template class which wraps over mutex, allowing for deferred locking, recursive locking, move of locking ownership, time-constrained attempt of locking 
*	and use in conjuction with condition variables. A unique_lock is NOT COPYABLE, but it is MOVABLE => only its copy semantics is deleted. The mutex is released at the end of the scope where the unique_lock 
*	is defined, following the RAII principle. Unlike lock_guard, it provides lock(), unlock(), try_lock_for() interface, meaning that it allows for on demand (explicit) unlocking and lock again operations, 
*	as well as for timed locking attempts. For these reasons the unique_lock is used in conjunction with conditions variables, as they have to unlock and lock the mutex for checking the status of the 
*	condition variable (notification received).
*
*	8. Locking options for lock_guard and unique_lock:
*		8.1 defer_lock - allows construction of an unique_lock that wraps a given mutex, but does not lock it. An explicit subsequent call to lock is required, which allows for acquiring of multiple locks 
*	using deadlock avoidance algorithm. It is used as option for unique_lock which allows for RAII lock-unlock approach, when dealing with 2 or more mutexes. Since lock_guard attempts to take ownership 
*	of the mutex immediately, defer_lock cannot be used in conjunction with lock_guard. The lock() call is made thereafter on the unique_lock objects, that wrapped the mutexes with defer_lock option.
*		8.2 adopt_lock - it assumes the calling thread already owns the mutex. Mainly used as an option to lock_guard, in order to bind the ownership of mutexes previously acquired, using deadlock avoidance 
*	algorithm, by a lock() call. Thereafter, the lock ownership is bound to (adopted) lock_guard objects, in order to ensure RAII unlocking. It can also be used with unique_lock, but it assumes the mutex is 
*	already locked.
*
*	9. recursive_mutex - it is a synchronization primitive preventing multiple threads from simultaneously accessing a shared data. Unlike mutex, once locked by a thread, the recursive_mutex can be 
*	locked and unlocked again by the same thread. This is useful when the shared data is accessed in a recursive fashion. The recursive_mutex is acquired when a successful call to lock or try_lock is done.
*	During this period, the thread can make additional calls to lock and unlock, for an unspecified number of times. The period of ownership ends when the number of unlocks matches the number of locks. 
*	Although unspecified, when the number of maximum lock is reached, any further call to lock returns system error.
*
*	10. timed_mutex - it is a synchronization primitive preventing multiple threads from simultaneously accessing a shared data. Likewise mutex, it offers exclusive, non recursive access to the shared data 
*	for the thread which successfully acquires it. In addition to mutex, it offers the possibility to try to lock the mutex, attempt tried for a specified amount of time or until a time point has been reached, 
*	using try_lock_for() and try_lock_until() respectively. These functions return true if the mutex has been locked during the specified time frame.
*
*	11. recursive_timed_mutex- it is a synchronization primitive preventing multiple thread from simultaneously accessing a shared data. Likewise timed_mutex, it offers the possibility to try to lock 
*	the mutex, attempt tried for a specified amount of time or until a time point has been reached, using try_lock_for() and try_lock_until() respectively. These functions return true if the mutex has 
*	been locked during the specified time frame. Additionally, likewise recursive_mutex, the recursive_timed_mutex can be locked() and unlocked() again by the thread that owns it. Unlike recursive_mutex, 
*	these attempts can be done in a timely manner.
*/

class Queue
{
    private:
    size_t length;
    vector<int> data;

    public:
    Queue() = default;
    Queue(const vector<int>& vecInt): length{vecInt.size()}, data{vecInt}{};
    size_t getLength() const {return length;}; 
    void queueSwap(Queue& q)
    {
        //enable finding std::swap via ADL, otherwise it would be considered a recursive call to the swap method implemented here,
        //if the method overloads swap for Queue objects
        //using std::swap;
        swap(data, q.data);
        swap(length, q.length);
    }
    void addElement(const int elem){ ++length; data.push_back(elem); };
    void printElements()
    {
        for(auto& elem : data)
            cout<<elem<<" ";
        cout<<endl;
    }
};

auto IncrementData(const unique_ptr<int>& uPtr, recursive_mutex& recMtxUPtr) -> void
{
    lock_guard<recursive_mutex> lckGuard{recMtxUPtr};
    if(uPtr)
    {
        ++(*uPtr);
        cout<<__FUNCTION__<<" Line: "<<__LINE__<<" value: "<<*uPtr<<endl;
    }
}

void mutexInsights()
{
	cout<<endl<<"Insights on std mutex"<<endl;

    cout<<endl<<">>>    lock_guard example   <<<"<<endl;

    auto threadLambda_LockGuard{  [](shared_ptr<HelperClass>& hc, mutex& hc_mutex)
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
    thread threadInst = thread(threadLambda_LockGuard, ref(sphc), ref(sphc_mutex));

    if(sphc_mutex.try_lock())
    {
        cout<<__LINE__<<" use_count: "<<sphc.use_count()<<endl;
        sphc_mutex.unlock();
    }

    joinThread(threadInst);

    //--------------unique_lock--------------
    cout<<endl<<">>>    unique_lock example   <<<"<<endl;
    int a{}, b{42};
    mutex mtx_a, mtx_b;

    auto preIncrement_Lambda{  [&mtx_a, &mtx_b](int& a, int& b, string threadName)
                                {
                                    unique_lock<mutex> ulk_mtx_a(mtx_a);
                                    ++a;
                                    //explicitly unlock the mutex, as it is not necessary anymore
                                    ulk_mtx_a.unlock();
                                    
                                    unique_lock<mutex> ulk_mtx_b(mtx_b, defer_lock);
                                    ulk_mtx_b.lock();
                                    ++b;
                                    ulk_mtx_b.unlock();
                                    
                                    //lock again mutex for a
                                    ulk_mtx_a.lock();
                                    ++a;
                                
                                    cout<<"[thread]: "<<threadName<<endl;
                                }
                              };

    thread preIncrementThread1{preIncrement_Lambda, ref(a), ref(b), "thread1"};
    thread preIncrementThread2{preIncrement_Lambda, ref(a), ref(b), "thread2"};

    cout<<"[before joining] a= "<<a<<" b = "<<b<<endl;
    joinThread(preIncrementThread1);
    joinThread(preIncrementThread2);

    cout<<"[after joining] a = "<<a<<" b = "<<b<<endl;

    //--------------adopt_lock and defer_lock---------------
    cout<<endl<<">>>    adopt_lock and defered_lock examples   <<<"<<endl;

    Queue q1{{1,2,3,4,5}};
    mutex mtx_q1;
    Queue q2{{-172,-183,-194}};
    mutex mtx_q2;

    auto swapQueuesLambda_AdoptLock{ [&mtx_q1, &mtx_q2](Queue& q1, Queue& q2)
                                        {
                                            //use std::lock to lock the mutexes using deadlock avoidance algorithm
                                            lock(mtx_q1, mtx_q2);
                                            //once locked, define RAII guards to ensure the mutexes are released at the end of this scope
                                            //as the mutexes were previously locked, use adopt_lock policy. The lock ownership is bound to lock_guard
                                            lock_guard<mutex> lkGuard_mtx_q1(mtx_q1, adopt_lock);
                                            lock_guard<mutex> lkGuard_mtx_q2(mtx_q2, adopt_lock);
                                            //swap the two queues in a thread safe manner
                                            q1.queueSwap(q2); 
                                        }
                                    };

    auto swapQueuesLambda_DeferLock{ [&mtx_q1, &mtx_q2](Queue& q1, Queue& q2)
                                        {
                                            //create unique_lock objects that ensure RAII lock-unlock takes place on mutexes which are subsequently locked
                                            unique_lock<mutex> lkGuard_mtx_q1(mtx_q1, defer_lock);
                                            unique_lock<mutex> lkGuard_mtx_q2(mtx_q2, defer_lock);
                                            //use std::lock to lock the unique_locks using deadlock avoidance algorithm
                                            lock(lkGuard_mtx_q1, lkGuard_mtx_q2);
                                            //swap the two queues in a thread safe manner
                                            q1.queueSwap(q2); 
                                        }
                                    };

    auto addValuesLambda_DeferLock{ [&mtx_q1, &mtx_q2](Queue& q1, Queue& q2)
                                        {
                                            //define RAII locking mechanisms using unique_lock and defer_lock policy in order to lock both mutexes 
                                            //subsequently, using lock. Defering locking works with unique_lock, but not with lock_guard, as it attempts to lock
                                            //the mutex immediately
                                            unique_lock<mutex> ulk_mtx_q1(mtx_q1, defer_lock);
                                            unique_lock<mutex> ulk_mtx_q2(mtx_q2, defer_lock);
                                            //explicitly lock the unique_locks, as after deferring lock, unique_lock.lock() is executed
                                            lock(ulk_mtx_q1, ulk_mtx_q2);
                                            q1.addElement(23);
                                            q2.addElement(-23);
                                        }
                                    };

    auto addValuesLambda_AdoptLock{ [&mtx_q1, &mtx_q2](Queue& q1, Queue& q2)
                                        {
                                            //explicitly lock the mutexes
                                            lock(mtx_q1, mtx_q2);
                                            //define RAII locking mechanisms using unique_lock and adopt_lock policy in order to adopt
                                            //mutex ownership and ensure RAII lock-unlock policy
                                            unique_lock<mutex> ulk_mtx_q1(mtx_q1, adopt_lock);
                                            unique_lock<mutex> ulk_mtx_q2(mtx_q2, adopt_lock);
                                            q1.addElement(23);
                                            q2.addElement(-23);
                                        }
                                    };
    
    //thread swapQueuesThread{swapQueuesLambda_AdoptLock, ref(q1), ref(q2)};
    //thread swapQueuesThread2{swapQueuesLambda_AdoptLock, ref(q1), ref(q2)};
    thread swapQueuesThread{swapQueuesLambda_DeferLock, ref(q1), ref(q2)};
    thread swapQueuesThread2{swapQueuesLambda_DeferLock, ref(q1), ref(q2)};
    //meanwhile attempt to push data to queues
    //thread addValuesThread{addValuesLambda_AdoptLock, ref(q1), ref(q2)};
    //thread addValuesThread{addValuesLambda_DeferLock, ref(q1), ref(q2)};

    cout<<" [before thread joined] q1 length: "<<q1.getLength()<<" q2 length: "<<q2.getLength()<<endl;
    q1.printElements();
    q2.printElements();

    if(swapQueuesThread.joinable())
    {
        cout<<" swapQueuesThread joining"<<endl;
        swapQueuesThread.join();
    }
    if(swapQueuesThread2.joinable())
    {
        cout<<" swapQueuesThread2 joining"<<endl;
        swapQueuesThread2.join();
    }
    // if(addValuesThread.joinable())
    // {
    //     cout<<" addValuesThread joining"<<endl;
    //     addValuesThread.join();
    // }

    cout<<" [after thread joined] q1 length: "<<q1.getLength()<<" q2 length: "<<q2.getLength()<<endl;
    q1.printElements();
    q2.printElements();

    //--------------scoped_lock---------------
    cout<<endl<<">>>    scoped_lock example   <<<"<<endl;

    auto swapQueuesLambda_ScopedLock{ [&mtx_q1, &mtx_q2](Queue& q1, Queue& q2)
                                        {
                                            //use scoped_lock lock the mutexes using deadlock avoidance algorithm whilst acquiring them in RAII fashion
                                            //Prior to c++17, this could only have been possible by using std::lock and lock_guard(adopt_lock) or unique_lock(deferred_lock)
                                            scoped_lock scopedLock(mtx_q1, mtx_q2);
                                            
                                            //swap the two queues in a thread safe manner
                                            q1.queueSwap(q2); 
                                            cout<<" [   thread] q1 length: "<<q1.getLength()<<" q2 length: "<<q2.getLength()<<endl;
                                            q1.printElements();
                                            q2.printElements();
                                        }
                                    };

    thread swapQueuesScopedLock{swapQueuesLambda_ScopedLock, ref(q1), ref(q2)};
    {
        scoped_lock scopedLock(mtx_q1, mtx_q2);
        q1.queueSwap(q2);
        cout<<" [before thread joined] q1 length: "<<q1.getLength()<<" q2 length: "<<q2.getLength()<<endl;
        q1.printElements();
        q2.printElements();
    }

    joinThread(swapQueuesScopedLock);

    cout<<" [after thread joined] q1 length: "<<q1.getLength()<<" q2 length: "<<q2.getLength()<<endl;
    q1.printElements();
    q2.printElements();

    //--------------recursive_mutex--------------
    cout<<endl<<">>>    recursive_mutex example   <<<"<<endl;
    
    recursive_mutex recMtx;
    unique_ptr<int> uPtrInt;

    auto lambda{
                [&recMtx, &uPtrInt](const string& thread_name)
                    {
                        lock_guard<recursive_mutex> lckGuard{recMtx};
                        if(uPtrInt)
                        {
                            ++(*uPtrInt);
                            cout<<thread_name<<" "<<__FUNCTION__<<" Line: "<<__LINE__<<" value: "<<*uPtrInt<<endl;
                        }
                        else
                        {
                            uPtrInt = make_unique<int>();
                            cout<<thread_name<<" "<<__FUNCTION__<<" Line: "<<__LINE__<<" value: "<<*uPtrInt<<endl;
                            this_thread::sleep_for(chrono::seconds(3));
                        }
                        IncrementData(uPtrInt, recMtx);
                        
                    ;}
                };

    thread th1{lambda, "thread1"}, th2{lambda, "thread2"};

    joinThread(th1);
    joinThread(th2);

    //--------------timed_mutex--------------
    cout<<endl<<">>>    timed_mutex example   <<<"<<endl;
    
    shared_ptr<int> sharedData{make_shared<int>(8)};
    timed_mutex timedMtx;

    auto threadFunc1 = [&sharedData, &timedMtx]
                        {
                            //if(timedMtx.try_lock_until(chrono::steady_clock::now() + chrono::seconds(2)))
                            if(timedMtx.try_lock_for(chrono::seconds(2)))
                            {
                                //if the mutex was acquired, adopt lock and own it in RAII manner
                                lock_guard<timed_mutex> lckGuard{timedMtx, adopt_lock};
                                this_thread::sleep_for(chrono::seconds(2));
                                ++(*sharedData);
                                cout<<this_thread::get_id()<<" "<<__LINE__<<" "<<__FUNCTION__<<" value: "<<*sharedData<<endl;
                            }
                            else
                            {
                                cout<<this_thread::get_id()<<" "<<__LINE__<<" "<<__FUNCTION__<<" couldn't lock timed_mutex"<<endl;
                            }
                            
                        };

    auto threadFunc2 = [&sharedData, &timedMtx]
                        {
                            this_thread::sleep_for(chrono::seconds(4));

                            //if(timedMtx.try_lock_until(chrono::steady_clock::now() + chrono::seconds(2)))
                            if(timedMtx.try_lock_for(chrono::seconds(2)))
                            {
                                //if the mutex was acquired, adopt lock and own it in RAII manner
                                lock_guard<timed_mutex> lckGuard{timedMtx, adopt_lock};
                                this_thread::sleep_for(chrono::seconds(2));
                                ++(*sharedData);
                                cout<<this_thread::get_id()<<" "<<__LINE__<<" "<<__FUNCTION__<<" value: "<<*sharedData<<endl;
                            }
                            else
                            {
                                cout<<this_thread::get_id()<<" "<<__LINE__<<" "<<__FUNCTION__<<" couldn't lock timed_mutex"<<endl;
                            }
                            
                        };

    auto dummyFunc = [&timedMtx]
                    {
                        this_thread::sleep_for(chrono::seconds(5));

                        //if(timedMtx.try_lock_until(chrono::steady_clock::now() + chrono::seconds(2)))
                        if(timedMtx.try_lock_for(chrono::seconds(2)))
                        {
                            lock_guard<timed_mutex> lckGuard{timedMtx, adopt_lock};
                            cout<<this_thread::get_id()<<" "<<__LINE__<<" "<<__FUNCTION__<<endl;
                        }
                        else
                        {
                            cout<<this_thread::get_id()<<" "<<__LINE__<<" "<<__FUNCTION__<<" dummyThread couldn't lock timed_mutex"<<endl;
                        }
                    };


    thread t1{threadFunc1}, dummyThread{dummyFunc}, t2{threadFunc2};

    joinThread(t1);
    joinThread(dummyThread);
    joinThread(t2);

}
