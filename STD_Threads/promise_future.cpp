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

class Functor
{
	private:
	const double pi{3.14159};
	double d;

	public:
	Functor() = default;
	explicit Functor(double dd):d{dd}{};
	double operator()(double in);
	Functor& operator=(const Functor& f);
	double getD() const{ return d;};
};

double Functor::operator()(double in)
{
	return in*pi;
}

Functor& Functor::operator=(const Functor& f)
{
	d = f.d;
	return *this;
}

void promise_futureInsights()
{
	cout<<endl<<"Insights on std promise and std future"<<endl;
	cout<<"     1. #include<future> header is used for accessing both promise and future"<<endl<<endl;
	
	cout<<"     2. promise<T> is a template class that can store a value/exception within a thread context. The stored value can be asynchronously retrieved"<<endl;
	cout<<"     by the calling thread using future<T>, instance of another template class. The future variable is built from the promise variable and is used to get the value set "<<endl;
	cout<<"     by the promise, even if the callee thread hasn't joined yet. "<<endl;
	cout<<"     That said, the {promise, future} objects can be used in conjunction for providing an async communication channel between 2 threads, "<<endl;
	cout<<"		that can allow for passing along a value, such as a push-pull system. Additionally, {promise, future} can be used for signaling state between"<<endl;
	cout<<"		2 threads, thus behaving like a barrier that allows for synchronization amongst the two."<<endl<<endl;

    cout<<"     3. Both future and promise are NOT COPYABLE => their copy semantics is deleted, so they can be passed as arguments using move semantics or references."<<endl<<endl;

	cout<<"     4. Both future and promise are usable only once. That is because {promise, future} share a given state, which is released after the result is "<<endl;
	cout<<"     retrieved from future or when the wait is over. When no state is associated, a std::future_error is thrown signaling no asscoiated state."<<endl;
	cout<<"     The shared state represents a neutral location where the value is actually stored (set by the promise, then retrieved by the future)."<<endl<<endl;

    cout<<"     5. The barrier-like mechanism for 2 threads leverages the shared state between promise and future. Concretely, promise<void> is used by "<<endl;
    cout<<"     the callee thread which has to call set_value() with no argument, such that the associated future's state becomes valid."<<endl;
	cout<<"		The caller thread block on future.wait() till valid is set to true."<<endl<<endl;
    
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

	cout<<"------------------------------------------------------------------------------------------------";
	cout<<endl<<"Insights on std::packaged_task"<<endl;
	cout<<"     1. std::packaged_task implementation also resides in the <future> header"<<endl<<endl;
	
	cout<<"     2. packaged_task<T> is a template class that wraps a callable target (std::function, regular functions, std::bind, lambdas) in order "<<endl;
	cout<<"     to call it asynchronously. The callbale target's returned value or exception is stored in a shared state that is accessed via a future object. "<<endl;
	cout<<"     The future object is obtained via a call to get_future() from the packaged_task instance => {packaged_task, future}, as a packaged_task has a shared state."<<endl<<endl;

	cout<<"     3.That said, packaged_task allows to move some logic of execution (functions, in a nutshell) to other threads. The logic has to be"<<endl;
	cout<<"		explicitly invoked using the calling operator ()."<<endl;
	cout<<"		A way to perceive packaged_task is that it consists of a std::function linked to a std::future."<<endl<<endl;

	cout<<"		4. A packaged_task can be moved to a thread or passed by reference using ref() "<<endl<<endl;

	cout<<"		5. As an alternative to {promise, future}, with  the promise being argument to a thread's entry point function, the thread's invokable "<<endl;
	cout<<"		target can be a packaged_task that can wrap a function that return a value. Furthermore, a packaged_task can wrap a function that takes"<<endl;
	cout<<"		a promise argument, each having its own future, such that 2 values can be retrieved from a callee thread = {packaged_task = {promise, future}, future}"<<endl<<endl;

	Functor f{};
	//packaged_task with functor target, with the argument being passed when the packaged_task is invoked, instead of explicit specifying it at binding
	//instead of specifying the operator() as bind argument, an instance of Functor can be passed along, as it is callable due to operator() overloading
	packaged_task<double(double)> packTaskFunctor{bind(f, placeholders::_1)};
	//packaged_task<double(double)> packTaskFunctor{bind(&Functor::operator(), &f, placeholders::_1)};
	
	//get associated future for the above packaged task
	future<double>  futurePackTaskFunctor = packTaskFunctor.get_future();
	//invoke the callable target
	packTaskFunctor(-1.0f);
	cout<<"future gotten from packed_task wrapping a functor: "<<futurePackTaskFunctor.get()<<endl;

	Functor f2{5.5};
	packaged_task<Functor&()> packTaskAssignment{bind(&Functor::operator=, &f, f2) };
	future<Functor&> futureFunctorAssignment = packTaskAssignment.get_future();
	packTaskAssignment();
	cout<<"future retrieved from packaged_task wrapping over assignment operator overload: "<<futureFunctorAssignment.get().getD()<<endl;

	shared_ptr<int> spi{make_shared<int>(8)};
	promise<int> promiseInt;
	//future associated with promise
	future<int> futurePromise = promiseInt.get_future();
	//packaged_task with lambda target
	packaged_task<int(const shared_ptr<int>&, promise<int>& p)> packTask{	[](const shared_ptr<int>& spi, promise<int>& p)
																			{
																				cout<<"inside thread callable target"<<endl;
																				int result{1};

																				if(spi)
																				{
																					cout<<"	shared_ptr value: "<<*spi<<" use count: "<<spi.use_count()<<endl;
																					result += *spi;
																				}

																				p.set_value(*spi*2);

																				return result;
																			}
																		};
	//future associated with packaged task
	future<int> futureTask = packTask.get_future();
	
	//define and launch thread
	thread packTaskThread{ref(packTask), ref(spi), ref(promiseInt)};
	//get futures
	auto futureTaskResult = futureTask.get();
	auto futurePromiseResult  = futurePromise.get();
	cout<<"future gotten from packaged_task: "<<futureTaskResult<<"| future gotten from promise that is parameter to packaged_task "<<futurePromiseResult<<endl;

	if(packTaskThread.joinable())
	{
		packTaskThread.join();
	}

	cout<<"------------------------------------------------------------------------------------------------";
	cout<<endl<<"Insights on std::async"<<endl;
	cout<<"     1. std::async implementation also resides in the <future> header"<<endl<<endl;
	
	cout<<"     2. async<T> is a template function that wraps a callable target (std::function, regular functions, std::bind, lambdas) in order "<<endl;
	cout<<"     to call it asynchronously, possibly in a separate thread, with the function's retruned value or exception being stored in a future<T> instance."<<endl;
	cout<<"     The future object is obtained via a call to get_future() from the async<T> instance => {async, future}, as an async call creates a shared state."<<endl<<endl;

	cout<<"		3. async function template takes as argument the target function followed by the arguments of the target function, one by one, as in the case of std::bind."<<endl;

	cout<<"     4.That function template provides an overload implementation that allows the specification of the launching policy, as the 1st parameter. If no launching policy"<<endl;
	cout<<"		is specified, the default one is applied, that is launch::async | launch::deferred, which means that the target may be executed either in another thread, either"<<endl;
	cout<<"		synchronously. Nevertheless, one fo the below policies can be explicitly specified."<<endl;
	cout<<"			- launch::async - is a policy of launching the target function of async on a separate thread, as if it was spawned using a call to thread c-tor. However, "<<endl;
	cout<<"		the difference from the case when spawned with thread() is that the returned value of the target function can be retrieved from the future associated with async."<<endl;
	cout<<"			- launch::deferred - is a policy of launching the target function using lazy evaluation: the execution occurs on the current thread, when future associated with"<<endl;
	cout<<"		async is invoked for getting the result."<<endl<<endl;

	cout<<"		5. The async function template retruns a future<T>, where T is the data type of the returned value from the target function."<<endl<<endl;

	cout<<"		6. Is it possible to specify a packaged_task as target of async, although it does not make much sense. Howeevr, in this case, the returned value is stored in the "<<endl;
	cout<<"		shared state of the packaged task. Thus, the future associated with the async will be future<void> and will hold no result."<<endl<<endl;

	//launch Functor using async and get the retruned future
	future<double> futureAsyncDeferred = async(launch::deferred, Functor(), 1.0f);
	//get the value from the returned future
	cout<<"get result from future returned by async launch::deferred: "<<futureAsyncDeferred.get()<<endl;

	auto resultFuture = async(launch::async, [&spi](char randomChar)
											{
												if(spi)
												{
													cout<<"inside async thread: captured shared_ptr value: "<<*spi<<" and random char: "<<randomChar<<endl;
												}
												return spi.use_count();
											},
											'/'
							 );
	//wait for the thread to make the result available. Otherwise, the below cout would be called before the result is ready
	resultFuture.wait();
	cout<<"get result from future returned from async launch::async: "<<resultFuture.get()<<endl;

	//declare a promise and the associated future
	promise<int> promiseIntPackedTask;
	future<int> futurepromiseIntPackedTask = promiseIntPackedTask.get_future();
	//define a packaged_task whose target function receives a promise argument and declare the associated future
	packaged_task<int(promise<int>&)> packedTask{[&spi](promise<int>& promiseIntPackedTask)
														{
															if(spi)
															{
																cout<<"		[packaged_task] inside async thread: captured shared_ptr value: "<<*spi<<endl;
																promiseIntPackedTask.set_value(++*spi);
															}
															return static_cast<int>(spi.use_count());
														}
													};
	auto futurePackedTask = packedTask.get_future();

	//use ref as it would be used in thread c-tor for arguments passed by reference to the target function. The packaged_task target can only be passed by reference or moved
	future<void> res = async(launch::async, ref(packedTask), ref(promiseIntPackedTask));
	res.wait();
	cout<<"future value from target packaged_task: "<<futurePackedTask.get()<<" future value from promise arg: "<<futurepromiseIntPackedTask.get()<<endl;

	//same as above but use function instead of packaged_task
	//declare a promise and the associated future
	promise<int> promiseIntFunction;
	future<int> futurePromiseIntFunction = promiseIntFunction.get_future();
	function<int(promise<int>&)> functionAsync{[&spi](promise<int>& promiseIntFunction)
														{
															if(spi)
															{
																cout<<"		[function] inside async thread: captured shared_ptr value: "<<*spi<<endl;
																promiseIntFunction.set_value(++*spi);
															}
															return static_cast<int>(spi.use_count());
														}
													};

	//use ref as it would be used in thread c-tor for arguments passed by reference to the target function. The packaged_task target can only be passed by reference or moved
	future<int> futureAsync = async(functionAsync, ref(promiseIntFunction));
	futureAsync.wait();
	cout<<"future value from async with target=function: "<<futureAsync.get()<<" future value from promise arg: "<<futurePromiseIntFunction.get()<<endl;

	//a bind can be sent as argument to async, or thread, as long as all args are specified => no placeholders
	auto result = async(launch::async, bind(&Functor::operator=, &f, f2));
	cout<<result.get().getD()<<endl;
}
