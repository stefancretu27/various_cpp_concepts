#include <iostream>

class Base
{
	public:
		void method()
		{
			std::cout<<"Base method"<<std::endl;
		};
	
};

class Derived1:public Base
{
	public:
		/*
		 * Define a method with the same name and return type as the one inherited from the base class, but with distinct parameters list
		 * It looks like an overload of an inherited method, but overloading does not have anything to do with inheritance.
		 * It is not an override, as the method does not have exactly the same signature with the one in the base class.
		 * So, when an instance of Derived would want to call method(), it will be shadowed by method(int), thus cannot be called like this: d.method()
		 * Instead, method() can be accessed from the Base part of the derived object using scoping operator: d.Base::method()
		 * */
		void method(int i)
		{
			std::cout<<"Derived method"<<std::endl;
		};
	
};

int main()
{
	std::cout<<"Base method called through base pointer"<<std::endl;
	Base *b = new Derived1();
	b->method();
	delete b;
	
	std::cout<<"Derived method that shadows base method called through derived pointer"<<std::endl;
	Derived1 *d = new Derived1();
	d->method(1);
	
	//as method(int) combines overriding and overloading =>shadowing <=>method() is shadowed in Derived and cannot be called from Derived1
	std::cout<<"Base method shadowed by derived method cannot be called through derived pointer: error no matching function call"<<std::endl;
	//d->method();
	//instead, be explicit when call the base class function
	std::cout<<"Base method shadowed by derived method called through derived pointer and scope resolution operator"<<std::endl;
	d->Base::method();
	
	delete d;
	
	return 0;
}
