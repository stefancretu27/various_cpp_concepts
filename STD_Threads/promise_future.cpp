#include "headers.hpp"

#include <future>

using namespace std;

void doPromiseFuture(promise<int16_t>& storePromise, promise<Garbage> promiseGarbage)
{
	storePromise.set_value(16);
	promiseGarbage.set_value(Garbage{-273.3});
}

void doWork(promise<void>& barrierLikePromise)
{
	//mock some work
	this_thread::sleep_for(100ms);
	
	//no explicit value is passed along, only the state is set to valid for the associated future
	barrierLikePromise.set_value();
}

void promise_futureInsights()
{
	cout<<endl<<"Insights on std promise and std future"<<endl;
	cout<<"     1. #include<future> header is used for accessing both promise and future"<<endl<<endl;
	
	cout<<"     2. promise<T> is a template class that can store a value/exception within a thread context. The value can be asynchronously retrieved by the calling thread"<<endl;
	cout<<"     using future<T>. The future variable is built from the promise variable and is used to retrieve the value passed by the promise, even if the producer thread hasn't joined yet."<<endl;
	cout<<"     That said, the {promise, future} objects can be used in conjunction for providing an async communication channel between 2 threads, that can allow for passing along a value,"<<endl;
	cout<<"     such as a push-pull system, or by signaling state, behaving like a barrier for 2 threads."<<endl<<endl;

    cout<<"     3. Both future and promise are not copyable => their copy semantics is deleted, so they can be passed as arguments using move semantics or references."<<endl<<endl;

	cout<<"     4. Both future and promise are usable only once. That is because {promise, future} share a given state, which is released after the result is retrieved from future or"<<endl;
	cout<<"     when the wait is over. When no state is associated, a std::future_error is thrown signaling no asscoiated state."<<endl<<endl;

    cout<<"     5. The barrier-like mechanism for 2 threads leverages the shared state between promise and future. Concretely, promise<void> is used by the callee thread which"<<endl;
    cout<<"     calls set_calue() with no argument, such that the associated future's state becomes valid. The caller thread block on future.wait() till valid is set to true."<<endl<<endl;
    
    cout<<"     6. Methods: "<<endl;
    cout<<"     - promise.get_future() is used to initialize a future that shares state with the calling promise"<<endl;
    cout<<"     - promise.set_value(arg) - is used in the callee thread to pass arg value to the caller thread"<<endl;
    cout<<"     "<<endl;
    cout<<"     - future.get() - is used to get the value, in the caller thread"<<endl;
    cout<<"     - future.valid() - checks if the promised value is made available"<<endl;
    cout<<"     - future.wait() - waits for the callee thread (the associated promise) to signal the state change, as it blocks till the result becomes available"<<endl<<endl;
    
    cout<<"     7. promise = {result, shared_state}, future{result, shared_state}. The result can contan a value, a exception or can be void."<<endl;
    cout<<"     Shared state contains state information:"<<endl;
    cout<<"     - make_ready: store the value in result and unblocks any thread waiting on future wait or get"<<endl;
    cout<<"     - release: the promise discards its reference to the shared state;"<<endl;
    cout<<"     - abandon: the shared state is made ready and the value is set to broken promise."<<endl<<endl;
    
	promise<int16_t> storePromise;
	future<int16_t> retrieveFuture = storePromise.get_future(); 
	promise<Garbage> promiseGarbage;
	future<Garbage> futureGarbage{promiseGarbage.get_future()};
	
	//start thread. The pass by reference promise is passed to thread ctor also by reference
	thread tPromiseFuture(doPromiseFuture, ref(storePromise), move(promiseGarbage));
	//retrieve the values from the associated futures
	cout<<" gotten promises: "<<retrieveFuture.get()<<" "<<futureGarbage.get().getD()<<endl;
	//join thread after asynchronously getting the value from the futures associated with the promise arguments 
	joinThread(tPromiseFuture);
	
	//use {promise, future} async communication channel as a barrier to sync the calling and the callee thread
	promise<void> barrierFor2Threads;
	future<void> futureBarrier = barrierFor2Threads.get_future();
	
	thread workerThread{doWork, ref(barrierFor2Threads)};
	if(futureBarrier.valid())
	{
		cout<<"worker thread has set state of the future barrier-like"<<endl;
	}
	//the caller thread blocks till the state of the future is changed
	futureBarrier.wait();
	joinThread(workerThread);
	
}
