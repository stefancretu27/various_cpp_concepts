#include "headers.hpp"

#include <condition_variable>

using namespace std;


void conditionVariableInsights()
{
	cout<<endl<<"Insights on std::condition_variable"<<endl;
    cout<<"     1. condition_variable is a template class implemented in header <condition_variable>"<<endl;

	cout<<"     2. Condition variable is a synchronization primitive that is used to block a thread, or more threads at the same time, until another "<<endl;
	cout<<"     thread both modifies a shared variable and notifies the condition_variable. That said, there can be only one thread attempting to access"<<endl;
    cout<<"     the shared variable and notify the others, whereas more threads are blocked and wait on the condition variable."<<endl<<endl;

    cout<<"     3. A thread that intends to notify on a condition variable, while accessing a critical section, has to:"<<endl;
	cout<<"         a. acquire a mutex (lock_guard, unique_lock etc)"<<endl;
    cout<<"         b. perform any read-modify-write operation on the shared variable, while the mutex is locked."<<endl;
    cout<<"         c. notify the other thread (via notify_one) or the other threads (notify_all) via the condition variable. Here, the mutex lock is not"<<endl;
    cout<<"     necessary, otherwise, the waiting thread has to wait for the notifying thread to release the mutex in order to proceed further."<<endl;
    cout<<"     Even if the shared variable is atomic, the access has to be guarded by a mutex, as the notification on the condition variable "<<endl;
    cout<<"     performs an unlock before sending the notification, then calls lock on the mutex, after the notification"<<endl<<endl;

    cout<<"     4. A thread that waits on a condition variable, while accessing a critical section, has to:"<<endl;
    cout<<"         a. acquire a unique_lock on the same mutex used by the notifying thread for accessing the same critical section. The unique_lock"<<endl;
    cout<<"         is required as multiple lock and unlock operations might be performed when waiting, in order to check for notifications."<<endl;
    cout<<"         b. either: - checks the condition variable in case it was notified already"<<endl;
    cout<<"                    - then executes wait, wait_for, wait_until on the condition variable. These operations release the mutex and "<<endl;
    cout<<"                    suspend the thread's execution until notify_all or notify_one is called on the same condition variable. "<<endl;
    cout<<"                    - when the condition variable is notified, a timeout expires or a spurious wakeup occurs, the thread is awakened, "<<endl;  
	cout<<"                    and the mutex is atomically reacquired.If the wakeup was spurious, recheck the condition variable and resume waiting."<<endl;
    cout<<"         or, use the predicated overload wait, wait_for, wait_until which take care of the 3 steps described above. Unlike the first"<<endl;
    cout<<"         implementation which only takes unique_lock argument, this overload takes as 2nd argument a function which can check if a given"<<endl;
    cout<<"         condition becomes true, in order to terminate the waiting. "<<endl<<endl;

	cout<<"     5. Since methods on condition variable (such as wait, wait_until, wait_for, notify_one, notify_all) firstly unlock the mutex,"<<endl;
	cout<<"     then perform the specific action, with the mutex being relocked afterwards, it can be inferred that these methods can be called"<<endl;
    cout<<"     concurrently on the same condition variable"<<endl<<endl;

    cout<<"     6. A condition variable instance is NOT COPYABLE or MOVABLE. As in the case of mutex, if it had been copyable, such a copy could "<<endl;
    cout<<"     be used on another thread, for the same critical section, which would mean that it could not wait/notify with the original instance,"<<endl;
    cout<<"     which could have lead to race conditions."<<endl<<endl;

    cout<<"     7. A call to wait() atomically unlocks the mutex, blocks the current thread's execution and adds the current thread to the list of "<<endl;
    cout<<"     threads that are waiting on *this, that is the given instance of condition variable. The thread's execution is resumed when notify_one "<<endl;
    cout<<"     or notify_all is called on the same condition variable, or can be awaken supriously. When the thread resumes, the lock is reaquired"<<endl;
    cout<<"     right before wait() exits"<<endl<<endl;

    cout<<"     8. A call to notify_one() causes a single thread to stop waiting on the condition variable, as it is awaken to check that the condition "<<endl;
    cout<<"     or predicate it was waiting for has been changed by the signaling thread. "<<endl;
    cout<<"     A call to notify_all() signals all the thread in the waiting queue of the given condition variable to wake up and to unblock."<<endl;

    cout<<"     In order to be able to receive the notification, the waiting thread must unlock the mutex when it is executing a call to wait(), thus "<<endl;
    cout<<"     the notifying thread does not need to keep the mutex locked while notifying. "<<endl;
    cout<<"     thread to unlock the mutex. However, notifying under lock might be performed when precise scheduling events are envisaged, which can"<<endl;
    cout<<"     lead for instance to termination of the waiting thread and destruction of the condition variable."<<endl<<endl;

    cout<<"     9. A lost wake up happens when the notifying threads sends its notification (notify_one/notify_all) before the waiting thread reaches"<<endl;
    cout<<"     its waiting state. Consequently, the waiting thread will wait forever."<<endl<<endl;

    cout<<"     10. A spurious wake up takes place when a thread that is waiting on a condition variable wakes up, as it was notified the condition"<<endl;
    cout<<"     (the predicate) it was waiting for has changed, but in fact it hasn't. This happens when there are more threads waiting on a condition "<<endl;
    cout<<"     variable and when the signaling thread notifies all, but only one thread will be able to access the critical section and see the"<<endl;
    cout<<"     condition it was waiting for was satisifed, thus it proceeds further with its computation and changes the condition back. That said, it"<<endl;
    cout<<"     is a race condition between threads in order to access the condition predicate's status and the first to wake up on the condition"<<endl;
    cout<<"     variable wins the race."<<endl<<endl;

    cout<<"     11. In order to avoid lost or spurious wake ups, an additional predicate has to be checked when executing a call to wait. The predicate"<<endl;
    cout<<"     represents a simple condition that verifies if the value of a shared variable has changed. The shared variable's purpose is to flag that"<<endl;
    cout<<"     a notification has been fired. If the notification is sent before the waiting thread reaches its waiting state, the additional condition"<<endl;
    cout<<"     is used to ensure a notification was sent. Without using an additional predicate, a call to wait sometimes works and sometimes blocks on"<<endl;
    cout<<"     waiting, therefore it is not reliable enough. If the predicate evaluates to false, the waiting thread will be put in waiting state"<<endl<<endl;

    //------------------------worker thread waits for main thread-------------------------------------
    cout<<"--------------------------------------worker thread waits for main thread (not much sense, but for syntax sake)"<<endl;
    //use a bool for checking its value in the predicate (lambda) passed to wait
    bool isProcessedByMainThread{false};

    auto workerThreadEntryPoint{ [&isProcessedByMainThread](int& sharedData, mutex& mtx, condition_variable& condVar)
                                                            {  
                                                                //attempt to modify the shared data after the main thread
                                                                //Step 1: lock the mutex using unique_lock
                                                                unique_lock<mutex> uniqueLockMtx{mtx};
                                                                //Step 2: wait on cond variable using the unique_lock
                                                                condVar.wait(uniqueLockMtx, [&isProcessedByMainThread]() 
                                                                                            {return isProcessedByMainThread==true;}
                                                                           );
                                                                //Step 3: access the critical section when the wait is over
                                                                cout<<" [worker thread] value before preincrement: "<<sharedData<<endl;
                                                                ++sharedData;
                                                                cout<<" [worker thread] value after preincrement: "<<sharedData<<endl;
                                                            }
                               };

    int sharedData{};
    mutex mtxSharedData;
    condition_variable condVarSharedData;

    //start thread
    thread workerThread{workerThreadEntryPoint, ref(sharedData), ref(mtxSharedData), ref(condVarSharedData)};

    //main thread modifies shared data before the  worker thread, and signals the change via cond variable
    {
        //Step 1: acquire the mutex when accessing the critical section
        //unique_lock<mutex> uniqueLck{mtxSharedData};
        lock_guard<mutex> lckGuard{mtxSharedData};
        //Step 2: access the critical section
        cout<<" [main thread] value before +27 change: "<<sharedData<<endl;
        sharedData += 27;
        isProcessedByMainThread = true;
        //Step 3: unlock mutex and notify on cond variable
        //uniqueLck.unlock();
    }
    condVarSharedData.notify_one();
    cout<<" [main thread] value after +27 change: "<<sharedData<<endl;

    //join worker thread
    joinThread(workerThread);

    //------------------------main thread waits for worker thread-------------------------------------
    cout<<"---------------------------------main thread waits for worker thread"<<endl;

    //use a bool for checking its value in the predicate (lambda) passed to wait
    bool isProcessedByWorkerThread{false};

    auto workerThreadTargetFunction{ [&isProcessedByWorkerThread](int& sharedData, mutex& mtx, condition_variable& condVar)
                                                            {  
                                                                //attempt to modify the shared data before the main thread
                                                                //Step 1: acquire the mutex
                                                                unique_lock<mutex> uniqueLckMtx{mtx};
                                                                //Step 2: access the critical section
                                                                cout<<" [worker thread] value before preincrement: "<<sharedData<<endl;
                                                                ++sharedData;
                                                                isProcessedByWorkerThread = true;
                                                                //Step 3: release mutex and notify on cond variable
                                                                uniqueLckMtx.unlock();
                                                                condVar.notify_one();
                                                                cout<<" [worker thread] value after preincrement: "<<sharedData<<endl;

                                                                uniqueLckMtx.lock();
                                                                ++sharedData;
                                                                cout<<" [worker thread] value after 2nd preincrement: "<<sharedData<<endl;
    
                                                            }
                               };

    int sharedInt{};
    mutex mtxSharedInt;
    condition_variable condVarSharedInt;

    //start thread
    thread workerThread2{workerThreadTargetFunction, ref(sharedInt), ref(mtxSharedInt), ref(condVarSharedInt)};
    
    //main thread modifies shared data after the worker thread, and waits on cond variable
    {
        //Step 1: lock the mutex using unique_lock
        unique_lock<mutex> uniqueLockMtx{mtxSharedInt};
        //Step 2: wait on cond variable using the unique_lock. This thread is blocked until notify_one or notify_all is called
        condVarSharedInt.wait(uniqueLockMtx, [&isProcessedByWorkerThread]() {return isProcessedByWorkerThread;});
        //Step 3: access the critical section when the wait is over
        cout<<" [main thread] value before +44 change: "<<sharedInt<<endl;
        sharedInt += 44;
        cout<<" [main thread] value after +44 change: "<<sharedInt<<endl;
    }

    //join worker thread
    joinThread(workerThread2);

    cout<<"-------------------main thread waits for worker thread without any predicate. It does not work the other way around as workerThread blocks."<<endl;

    auto workerThreadFunction_NoPredicate{ [&isProcessedByWorkerThread](int& sharedData, mutex& mtx, condition_variable& condVar)
                                                            {  
                                                                //attempt to modify the shared data before the main thread
                                                                //Step 1: acquire the mutex
                                                                unique_lock<mutex> uniqueLckMtx{mtx};
                                                                //Step 2: Access the critical section
                                                                cout<<" [worker thread] value before preincrement: "<<sharedData<<endl;
                                                                ++sharedData;
                                                                cout<<" [worker thread] value after preincrement: "<<sharedData<<endl;
                                                                //Step 3: unlock unique_lock and notify on cond variable
                                                                uniqueLckMtx.unlock();
                                                                condVar.notify_one();

                                                                uniqueLckMtx.lock();
                                                                ++sharedData;
                                                                cout<<" [worker thread] value after 2nd preincrement: "<<sharedData<<endl;
                                                            }
                               };

    int dataInt{};
    mutex mtx;
    condition_variable condVar;

    //start thread
    thread workerThreadNoPredicate{workerThreadFunction_NoPredicate, ref(dataInt), ref(mtx), ref(condVar)};
    
    //main thread modifies shared data after the worker thread, and waits on cond variable
    {
        //Step 1: lock the mutex using unique_lock
        unique_lock<mutex> uLockMtx{mtx};
        //Step 2: wait on condition variable using unique_lock
        condVar.wait(uLockMtx);
        //Step 3: access the critical section
        cout<<" [main thread] value before -23 change: "<<dataInt<<endl;
        dataInt -= 23;
        cout<<" [main thread] value after -23 change: "<<dataInt<<endl;
    }
    //join worker thread
    joinThread(workerThreadNoPredicate);
}
