#include "headers.hpp"

#include <atomic>

using namespace std;

/*
* 	1. <atomic> header is used for accessing atomic operations(functions) and the atomic template class, being introduced in C++11
*	
*	2. atomic<T> is a template class that allows for creating objects which can be accessed by multiple threads, for read-modify-write operations, without leading to race condition. 
*	   This is called atomic access and it involves a well defined (sequential) access of multiple threads to a given atomic variable. That said, a thread cannot access the atomic 
*	   variable whilst another thread is accessing it, as the atomic operation is indivisible. Therefore, no locking mechanism is required when an atomic variable is accessed, as the atomic
*	   operations are thread safe by default.
*	
* 	3. The underlying data type can be:
*		3.1 any scalar type 
* 		3.2 trivially copyable classes/structs. A class is trivially copyable if its dtor, copy and move semantics are default or implicitly generated => they are not user provided.
*		Additionally, the class should not contain any virtual members, or to virtually inherrit (diamond problem). That said, basic operations on such objects can be completed in an 
*		indivisible manner. On top of that, atomic<T> cannot be instantiated with tuples, pairs and containers. Such classes are not trivially copyable.
*
*	4. atomic<T> is NOT COPYABLE or MOVABlE => its copy and move semantics are deleted. If copy semantics would have been allowed, it would lead to the situation that 2 threads could 
*	   attempts to access same memory location if T is a raw pointer => race condition.
*
*	5. When default constructing an atomic variable, the default ctor of class T should not perform any initialization. In other words, it should be implicit or default, in order to 
*	    follow the trivial approach of the overall class design.
*
*	6. Since the underlying data type must be trivial, it means atomic<T> cannot be instantiated for complex classes/structs. However, if there is needed an atomic access for such a complex 
*	class/struct, the preferred approach is to make some members atomic, after a proper identification on which ones are suitable and require atomic operations.
*
*	7. Methods: 
*		- default ctor - calls the default ctor of T class, which must be default or implicit.
*		- ctor with one parameter - it performs initialization of the underlying data, non atomically. For classes/structs with more fields, it takes as argument anonymous or existing objects.
*		- operator=(val)- assigns the new value to the underlying data. It does not perform assignment between atomic variables and the rhs operator must be a value, not another atomic variable
*		- T val = load() - returns the underlying value"<<endl;
*		- T old = exchange(T new) - returns old value and replaces it with the new one
*		- store(T val) - stores the val in the atomic variable, discarding the old one
*		- pre/post increment and decrement, that work only with integral types and pointers
*		- T old = fetch_add(val)/fetch_sub(val) - returns the existing value and adds/substracts val from it.
*    
*	8. The atomic operations are completed by default using the sequential consistent memory model for operations ordering (memory_order_seq_cst).
*	   This implies the all atomic operations on a given atomic variable are executed with a global ordering (such as a sequence of operations).
*		- memory_order_release - used in conjunction with store, performs a release after writting the value to the atomic variable, such that all preeceding store operations on the same 
*		  variable are performed before the current one.
*		- memory_order_acquire - used in conjunction with load, performs a lock before retrieving the data value, such that all subsequent load on the same atomic variable are executed after 
*		  the current one
*/

void thread_entry_point(const shared_ptr<int>& spi, atomic<uint_least16_t>& aUInt16)
{
	if(spi)
	{
		cout<<"value pointed to: "<<*spi<<" shared_ptr.use_count: "<<spi.use_count()<<" "<<this_thread::get_id()<<endl;
	}
	
	//use default memory model that ensures sequential consistency
	//get the previous value then atomically increment by 12. Quite similar to exchange, but it does an extra add
	uint_least16_t result = aUInt16.fetch_add(12, memory_order_seq_cst);
	cout<<"value from atomic int after fetch_add was called: "<<aUInt16.load()<<" previous value stored in atomic variable: "<<result<<endl;
	//atomically decrement
	--aUInt16;
	cout<<"value from atomic int after pre decrement was called: "<<aUInt16.load()<<endl;
}

struct Trash
{
	int i;
	float f;
	
	Trash() = default;
	Trash(int ii, float ff):i{ii}, f{ff}
	{
		cout<<"Trash non default ctor"<<endl;
	};
};

void atomicInsights()
{
	cout<<endl<<"Insights on std atomic"<<endl;
	cout<<"     <<endl<<endl;

	//declare atomic variable with initialization upon creation
	atomic<uint_least16_t> aUInt16{33};
	//declare atomic variable and initialize it afterwards, using the assignment operator of the atomic class
	atomic<char16_t> aChar16;
	aChar16 = 'd';
	cout<<static_cast<char>(aChar16.load())<<endl;
	aChar16 = 'q';
	cout<<static_cast<char>(aChar16.load())<<endl;
	
	//the operation below is not allowed as the rhs operand is an atomic variable and not a value, leading to call of the deleted assignment operator of atomic class
	//atomic<char16_t> aChar16New;
	//aChar16New = aChar16;
	//cout<<static_cast<char>(aChar16New.load())<<endl;
	
	//declare atomic variable to a trivially copyable class, using the default ctor of the atomic class (and of the T class, implicitly), then exchange its value.
	atomic<Garbage> aDefault{};
	Garbage oldValue = aDefault.exchange(Garbage{3.14159});
	cout<<" old value: "<<oldValue.getD()<<" atomic variable value after exchange: "<<aDefault.load().getD()<<endl;
	
	//if the non default c-tor of the T class is explicit, the following declaration + initialization won't work
	//atomic<Garbage> aNonDefault{3.14159};
	atomic<Trash> aNonDefault{Trash{23, -9.98}};
	//retrieve the underlying object using load
	Trash underlyingObj = aNonDefault.load();
	cout<<" Trash values after load from atomic variable to an anonymous object: "<<underlyingObj.i<<" "<<underlyingObj.f<<endl;

	Garbage garbageEuler{2.7182};
	//create and atomic variable and initialize it with an existing object. The, atomically replace the old value with a new one
	atomic<Garbage> aCopy{garbageEuler};
	aCopy.store(Garbage{-273.2});
	cout<<" value retrieved from atomic variable after store: "<<aCopy.load().getD()<<endl;
	
	//declare atomic variable whose underlying type is pointer to int. Upon atomic variable's declaration allocate and initialize memory pointed to by the pointer
	atomic<int*> aPInt{new int[5]{15,16,17,18,19}};
	int* pInt = aPInt.exchange(new int[5]{0,1,2,3,4});
	
	cout<<"values of the dynamically allocated arrays held in atomic variable to pointer to int "<<endl;
	for(size_t idx{}; idx < 5; ++idx)
	{
		cout<<" old value: "<<*(pInt+idx)<<" new value: "<<aPInt.load()[idx]<<endl;
	}
	
	//use atomic variable to pointer to int to dereferntiate the pointer
	cout<<" dereferentiate atomic variable to pointer to access 3rd element: "<<*(aPInt+2)<<endl;
	
	shared_ptr<int> spi{make_shared<int>(8)};
	//pass atomic variable to thread entry point only by reference, as there is no copy or move semantics
	thread t1, t2;
	t1 = thread(thread_entry_point, ref(spi), ref(aUInt16));
	t2 = thread(thread_entry_point, spi, ref(aUInt16));
	
	joinThread(t1);
	joinThread(t2);
	
	delete [] aPInt;
	aPInt = nullptr;
	
	delete [] pInt;
	pInt = nullptr;
	
}
