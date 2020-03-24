#include <iostream>

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
	public:
	Base(){ctor();};
	inline void func(){do_f();};
	~Base()
	{
		//std::cout<<"Base d-tor"<<std::endl;
		dtor();
	};
	void method()
	{
		std::cout<<"Base method"<<std::endl;
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
	Derived1(){ctor();};
	//even if commented, still works fine as vptr from public Base portion points to Derived1 vtable
	//inline void func(){do_f();};	
	~Derived1()
	{
		//std::cout<<"Derived1 d-tor"<<std::endl;
		dtor();
	}
	void method(int i)
	{
		std::cout<<"Derived method"<<std::endl;
	};
	
};

int main()
{
	std::cout<<"Delete derived through base pointer"<<std::endl;
	Base *b = new Derived1();
	b->func();
	b->method();
	delete b;
	
	std::cout<<"Delete derived thorugh derived pointer"<<std::endl;
	Derived1 *d = new Derived1();
	d->func();
	d->method(1);
	
	//as method(int) combines overriding and overloading =>shadowing <=>method() is shadoweld in Derived and cannot be called from Derived1
	//d->method();
	//instead, be explicit when call the base class function
	d->Base::method();
	
	delete d;
	
	return 0;
}
