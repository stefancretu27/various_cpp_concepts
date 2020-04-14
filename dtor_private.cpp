#include <iostream>

/*
 * http://www.gotw.ca/publications/mill18.htm
 * According to Herb Sutter's article about virtuality, the virtual methods should be private as much as possible, protected in some cases and public rarely, excepting the destructors.
 * He mentiones the C++ standard mostly follows this pattern, called Template Method.
 */
  
 
 /*
 * Template method: 
 * Nevertheless, the public methods specify the interface the given class provides to the outside world/clients.
 * Whereas the virtual methods specify the customizable behavior for that class, as the classes inheriting from it can adapt that behavior to their own needs.
 * That said, the outside world does not have to known it uses an interface that is customizable, as it is just interested in using an interface to a certain behavior.
 * Therefore, interfacing and customization should not be interleaved.
 * Only if derived classes need to invoke the base implementation of a virtual function, make the virtual function protected.
 */
 
/*
 * Implementation and advantages:
 * Concretely, for each customizable behavior there is a private/protected virtual method which is called by at least one public non virtual method.
 * The private virtual methods are accessed through each class virtual pointer and table. 
 * That said, if the base class has all its virtual methods private, they will not be inherited by children classes, but still accessible via vptr. 
 * So, if not overriden by the children class, the base class virtual methods will be called when a derived class invokes a public method inhrited from the base class.
 * In conclusion, regardless the virtual methods are public or private, the principles of vptr and vtable stay the same.
 * 
 * That said, first of all, this allows for insertion of further logic in the public method that can condition the private virtual method's invocation. This logic can be further pre and post conditions, instrumentations. 
 * 
 * Secondly, a public method can insert conditions on calling distinct private virtual methods within it, depending on some paramters, for instance. 
 * That said, it is not a 1-to-1 matching between private virtual methods and public methods.
 * 
 * Third in a row, these instrumentations are inherited by all members so it leads to less code duplication, in case the children class use similar logic. 
 * Consequently, it separates the customizable behavior of each class, in the inheritance chain, from the common behavior they can share (that is the instrumentation in the public methods).
 * 
 * Fourth in a row, no overhead is introduced. In the case no extra instrumentation is added within the public method, the compilers ellide the extra method call by merging them.
 */ 
 
 /*
  * Order of execution when calling constructor:
  * 1) calls parent's c-tor
  * 2) sets vptr to this class vtable
  * 3) executes member initializer list
  * 4) executes the c-tor code
  * 
  * Order of execution when calling destructor
  * 1) sets vptr to point to this class vtable
  * 2) executes the d-tor code
  * 3) destroys member data
  * 4) calls parent's d-tor
  * 
  * Given the above, a virtual destructor should be always public.
  * Otherwise, if template method is used for d-tors, when called by a pointer to a base class, for a derived class object, it will always call the base class d-tor, as the d-tor had already set vptr to base vtable
  */

class Base
{
	private:
		virtual void ctor()
		{
			std::cout<<"Base ctor"<<std::endl;
		};
		virtual void dtor()
		{
			std::cout<<"Base clean"<<std::endl;
		};
		virtual void do_f()
		{
			std::cout<<"Base do_f"<<std::endl;
		};
		virtual void do_f2()
		{
			std::cout<<"Base do_f2 which will no be overriden"<<std::endl;
		};
			
	public:
		Base()
		{	
			//call private virtual method
			ctor();
		};
		
		inline void func()
		{
			//call private virtual method
			do_f();
			do_f2();
		};
		
		~Base()
		{
			//call private virtual method
			dtor();
		};
};

class Derived1:public Base
{
	private:
		virtual void ctor()
		{
			std::cout<<"Derived1 ctor"<<std::endl;
		};
		virtual void dtor()
		{
			std::cout<<"Derived1 clean"<<std::endl;
		};
		virtual void do_f()
		{
			std::cout<<"Derived1 do_f"<<std::endl;
		};	
	public:
		Derived1()
		{
			//call private virtual method
			ctor();
		};
		//even if commented, still works fine as vptr from public Base portion points to Derived1 vtable
		//inline void func(){do_f();};	
		~Derived1()
		{
			//call private virtual method
			dtor();
		}
};

int main()
{
	std::cout<<"Delete derived through base pointer"<<std::endl;
	Base *b = new Derived1();
	b->func();
	delete b;
	
	std::cout<<std::endl<<std::endl<<"Delete derived thorugh derived pointer"<<std::endl;
	Derived1 *d = new Derived1();
	d->func();
	delete d;
	
	return 0;
}
