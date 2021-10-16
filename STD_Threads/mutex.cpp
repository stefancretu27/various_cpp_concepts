#include "headers.hpp"

#include <vector>

using namespace std;

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

void mutexInsights()
{
	cout<<endl<<"Insights on std mutex"<<endl;
    cout<<"     1. mutex is a template class implemented in header <mutex>"<<endl;

	cout<<"     2. Mutex is a synchronization primitive that ensures exclusive access (atomic access) to a shared resource amongst multiple threads "<<endl;
	cout<<"     attempting to access it. When a thread acquires a mutex, it becomes the owner of that mutex and is the only  responsible for unlocking it."<<endl;
    cout<<"     A thread attempting to acquire a mutex must not own already the same mutex, otherwise it leads to undefined behavior, possibly deadlock."<<endl<<endl;

    cout<<"     3. Since a mutex provide exclusive access to a given shared resource, when dealing with multiple shared resources, the approach is to"<<endl;
	cout<<"     have a mutex for each shared resource on which atomic access is required. Using a given mutex for more shared variables would be"<<endl;
    cout<<"     ambiguous and could lead to deadlocks or undefined behaviors. lock(mutex1, mutex2, ...mutexn) can be used to lock at least 2 mutexes"<<endl<<endl;

    cout<<"     4. A mutex instance is NOT MOVABLE or COPYABLE => mutex class does not implement any copy or move semantics. If a mutex would have been "<<endl;
    cout<<"     copyable, a copy instance could be created on another thread. Thereafter, on the initial instance, but also on the copy instance"<<endl;
    cout<<"     lock operations would be possible. If both the initial instance and the copy should guard the access to the same shared resource, "<<endl;
    cout<<"     each thread would end with locking distinct mutexes (as the copy is another different instance), thus not preventing race condition"<<endl;
    cout<<"     In the end, in order to properly guard a critical section, all threads should use the same mutex when accessing it, such that each one"<<endl; 
	cout<<"     can check if the mutex is locked or not. The mutex locking status is visible to all thread."<<endl<<endl;

	cout<<"     5. A mutex is owned when it is successfully acquired using one of the following:"<<endl;
	cout<<"         1. mutex.lock() - acquire mutex. If already acquired by another thread, this call blocks here until the mutex is released."<<endl;
    cout<<"         2. mutex.try_lock() - tries to lock the mutex and returns immediately. On success returns true, false is returned otherwise."<<endl<<endl;
    cout<<"      When lock or try_lock are used to acquire a mutex, it must be released explicitly using mutex.unlock(). Attempting to unlock a mutex"<<endl;
    cout<<"     that is not locked, leads to undefined behavior."<<endl<<endl;

    cout<<"     6. lock_guard<std::mutex> lg(mutex) - it is a template class which wraps over mutex, allowing for acquiring a mutex in RAII fashion, "<<endl;
    cout<<"     within the scope where the lock_guard is defined. When lock_guard is destroyed, at the end of the scope where it is defined,"<<endl;
    cout<<"     its d-tor automatically calls unlock(). A lock_guard is NOT COPYABLE or MOVABLE => both copy and move semantics are deleted."<<endl<<endl;

    cout<<"     7. unique_lock<std::mutex> lg(mutex) - it is a template class which wraps over mutex, allowing for deferred locking, recursive locking "<<endl;
    cout<<"     move of locking ownership, time-constrained attempt of locking and use in conjuction with condition variables."<<endl;
    cout<<"     A unique_lock is NOT COPYABLE, but it is MOVABLE => only its copy semantics is deleted. The mutex is released at the end of the scope"<<endl;
    cout<<"     where the unique_lock is defined, following the RAII principle."<<endl;

    cout<<"     8. Locking options:"<<endl;
    cout<<"      - defer_lock - allows construction of an unique_lock that wraps a given mutex, but does not lock it. An explicit subsequent call to"<<endl;
    cout<<"     lock is required, which allows for acquiring of multiple locks using deadlock avoidance algorithm. It is used as option for unique_lock"<<endl;
    cout<<"     which allows for RAII lock-unlock approach, when dealing with 2 or more mutexes. Since lockguard attempts to take ownership of the mutex"<<endl;
    cout<<"     immediately, defer_lock cannot be used in conjunction with lock_guard. The lock is called on unique_lock objects."<<endl;
    cout<<"      - adopt_lock - it assumes the calling thread already owns the mutex. Mainly used as an option to lock_guard, in order to bind the ownership "<<endl;
    cout<<"     of mutexes previously acquired using deadlock avoidance algorithm, by a lock() call. Thereafter, the lock ownership is bound to (adopted) "<<endl;
    cout<<"     lock_guard objects, in order to ensure RAII unlocking.It can also be used with unique_lock, but it assumes the mutex is already locked."<<endl<<endl;
    
    cout<<endl<<">>>    lock_guard stuff   <<<"<<endl;

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
    cout<<endl<<">>>    unique_lock stuff   <<<"<<endl;
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
    cout<<endl<<">>>    adopt_lock and defered_lock stuff   <<<"<<endl;

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

}
