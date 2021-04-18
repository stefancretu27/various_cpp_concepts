//for size_t
#include <cstdio>
//for move
#include <utility>
#include <iostream>
#include <vector>

#include "DerivedImpl.hpp"

//using namespace std;

int main()
{
	cout<<"Create Base object that overrides inherited pure virtual, virtual and normal methods from Root abstract class"<<endl;
	Base baseInst{"Base instance"};
	baseInst.pureVirtualMethod();
	baseInst.virtualMethod();
	baseInst.method();

	cout<<endl<<"Create reference to Root that refers to Base instance"<<endl;
	Root& rootBaseRef{baseInst};
	rootBaseRef.pureVirtualMethod();
	rootBaseRef.virtualMethod();
	rootBaseRef.method();

	cout<<endl<<"Create Derived object that overrides inherited pure virtual, virtual and normal methods from Base  class"<<endl;
	DerivedImpl derivedInst{};
	derivedInst.pureVirtualMethod();
	derivedInst.virtualMethod();
	derivedInst.method();

	cout<<endl<<"Copy assign Derived instance to Base instance"<<endl;
	baseInst = derivedInst;
	baseInst.pureVirtualMethod();
	baseInst.virtualMethod();
	baseInst.method();

	cout<<endl<<"Create reference to Root that refers to DerivedImpl instance"<<endl;
	Root& rootDerivedRef = derivedInst;
	rootDerivedRef.pureVirtualMethod();
	rootDerivedRef.virtualMethod();
	rootDerivedRef.method();

	cout<<endl<<"Polymorphism and virtuality"<<endl;
	cout<<"     1. Overriding: The method must keep same signature as it is defined in the most Base class it appears:"<<endl;
	cout<<"     	- same name,"<<endl;
	cout<<"     	- same return type,"<<endl;
	cout<<"     	- same parameters types and same number of parameters"<<endl;
	cout<<"     	- same constness."<<endl<<endl;

	cout<<"     2. Polymorphism is the ability of a method call to be resolved to the most Derived implementation of that method. Polymorphism is achieved as follows"<<endl;
    cout<<"     	- the method is declared as <<virtual>> in the most Base class"<<endl;
	cout<<"     	- the method is overriden in Derived classes, so it has different implementations in the inheritance chain. "<<endl;
	cout<<"     	- a pointer or reference to Base class points/refer to a Derived class and the virtual method is invoked via this pointer/ref. Thus, it is leveraged the pointer"<<endl;
	cout<<"     type compatibility between Base and Derived, that does not imply object slicing If Derived is assigned to a Base instance,  object slicing takes place and all methods"<<endl;
	cout<<"     from Derived, including the virtual ones, would not become accessible via this Base instance, as Derived instance is rescoped to Base type, hence no polymorphism."<<endl;
	cout<<"     	- late binding is used to resolve method call at runtime. It is achieved through pointers to methods, as a class that declares a virtaul method, gets"<<endl;
	cout<<"     a public virtual pointer that points to a static array of pointers to functions declared as virtual. The Derived classes inherrit the vptr and the vtable."<<endl;
	cout<<"     If an inherited virtual method is overriden in the Derived class, its entry in the Derived vtable is updated. Thus, at runtime, when a virtual method is invoked"<<endl;
	cout<<"     the vptr is dereferenced to access the vtable and the apropriate entry for the virtual method is found and the call is resolved to it. Binding is the process"<<endl;
	cout<<"     that converts variables and functions into addresses. For pointers, it always takes place at runtime, referred to as late/dynamic binding"<<endl<<endl;

	cout<<"     3. Constructors cannot be declared as virtual (error is thrown otherwise). Anyway, if it had been possible, when the c-tor is called, the vptr is set to point"<<endl;
	cout<<"     to current's class vtable, so the c-tor of the current class couldn't be called via vptr, but the one of its Base class, as the Derived part is not built yet."<<endl;
	cout<<"     Steps executed by constructor:"<<endl;
	cout<<"     	1. call parent's class c-tor, to construct the Base part"<<endl;
	cout<<"     	2. sets vptr inherited from parent to point to current class vtable. Thus, virtual methods called within c-tor ca resolve to current class versions."<<endl;
	cout<<"     	3. executes member initializer list"<<endl;
	cout<<"     	4. executes instructions specified in c-tors body implementation"<<endl<<endl;

	cout<<"     4. Destructors must be declared as virtual whean dealing with polymorphism. This was is ensured the resources pointed to by Base pointer or ref are properly"<<endl;
	cout<<"     deallocated."<<endl;
	cout<<"     Steps executed by destructor:"<<endl;
	cout<<"     	1. sets vptr inherited from parent to point to current caller class vtable. This means all virtual methods called in d-tor will call Base versions."<<endl;
	cout<<"     	2. executes instructions specified in d-tor body implementation"<<endl;
	cout<<"     	2. calls members' destructors to deallocate them."<<endl;
	cout<<"     	4. call parent's class d-tor, to destory the Base part"<<endl<<endl;

	cout<<"     5. The override qualifier can be specified after method's name, in Derived classes that override virtual methods inherited from Base class, in order to enforce"<<endl;
	cout<<"     a correct overriding, such that one of the four criteria above is not violated. Otheriwse, error is thrown at compile time. This way is ensured that when a"<<endl;
	cout<<"     Derived instance is passed to a Base ref/pointer which would call the virtual the virtual method, no ambiguous call or no matching call is issued, as the"<<endl;
	cout<<"     incompatiblity is detected when the class is implemented, not when the class is used."<<endl<<endl;

	cout<<"     6. The final qualifier can be specified after method's name, in Derived classes that override virtual methods inherited from Base class, in order to ensure"<<endl;
	cout<<"     that virtual method will not be overriden further, down in the inheritance chain."<<endl<<endl;

    
	return 0;
}
