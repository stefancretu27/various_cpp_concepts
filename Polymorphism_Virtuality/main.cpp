//for size_t
#include <cstdio>
//for move
#include <utility>
#include <iostream>
#include <vector>

#include "headers.hpp"

using namespace std;

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
	rootBaseRef.Root::pureVirtualMethod();

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
	//rootDerivedRef.Base::pureVirtualMethod();  //error: ‘Base’ is not a base of ‘Root’
	rootDerivedRef.Root::pureVirtualMethod();

	cout<<endl<<"reference to Base class refering Derived object calls pure virtual method, then uses scoep resolution to access the pure virtual methods from Base and Root"<<endl;
	Base& baseDerivedRef = derivedInst;
	baseDerivedRef.pureVirtualMethod();
	baseDerivedRef.Base::pureVirtualMethod();
	baseDerivedRef.Root::pureVirtualMethod();

	cout<<endl<<"dynamic cast Derived references from references to Root and Base that refer to Derived"<<endl;
	DerivedImpl& dynamicCastDerivedFromRoot = dynamic_cast<DerivedImpl&>(rootDerivedRef);
	dynamicCastDerivedFromRoot.method();
	DerivedImpl& dynamicCastDerivedFromBase = dynamic_cast<DerivedImpl&>(baseDerivedRef);
	dynamicCastDerivedFromBase.method();

	cout<<endl<<"dynamic cast Base reference from reference to Root that refer to Derived"<<endl;
	Base& dynamicCastBaseFromRoot = dynamic_cast<Base&>(rootDerivedRef);
	dynamicCastBaseFromRoot.method();

	cout<<endl<<"Dynamic polymorphism and virtuality"<<endl;
	cout<<"     1. Overriding: The method can have distinct implementations within the inheritance chain but must preserve the:"<<endl;
	cout<<"     	- same name,"<<endl;
	cout<<"     	- same return type,"<<endl;
	cout<<"     	- same parameters types and same number of parameters"<<endl;
	cout<<"     	- same constness."<<endl<<endl;

	cout<<"     2. Dynamic polymorphism is the ability of a method call to be resolved to the most Derived implementation of that method. Polymorphism is "<<endl;
    cout<<"     achieved as follows:"<<endl;
	cout<<"			- the method is declared as <<virtual>> in the most Base class"<<endl;
	cout<<"     	- the method is overriden in Derived classes, so it has different implementations in the inheritance chain. "<<endl;
	cout<<"     	- a pointer or reference to Base class points/refer to a Derived class and the virtual method is invoked via this pointer/ref."<<endl;
	cout<<"		Thus, it is leveraged the pointer type compatibility between Base and Derived, that does not imply object slicing. If Derived were"<<endl;
	cout<<"     assigned to a Base instance, object slicing takes place and all methods from Derived, including the virtual ones, would not become"<<endl;
	cout<<"     accessible via this Base instance, as the vptr is not subject of assignment or copy, hence no polymorphism"<<endl<<endl;

	cout<<"     3. Even though a Derived class overrides virtual methods inherrited from Base, the Base implementations of those virtual methods are "<<endl;
	cout<<"     available via scope resolution, as it happens with any other public or protected method from Base. This is possible because, when a "<<endl;
	cout<<"     Derived object is constructed in phases, the Base part comes with all its protected and public methods, no matter they are virtual. "<<endl;
	cout<<"     Additionally, when a pointer/reference to Base points/refers to a Derived instance, it can only call virtual methods from Derived, "<<endl;
	cout<<"     via vptr and vtable, and virtual methods from Base, via scope resolution. If there is an intermediary class in this inheritance chain,"<<endl;
	cout<<"     that Base pointer/ref cannot call any of its methods via scope resolution. It can only call virtual methods from intermediary class that "<<endl;
	cout<<"     are not overriden in Derived class."<<endl<<endl;

	cout<<"     4. Constructors cannot be declared as virtual (error is thrown otherwise). Anyway, if it had been possible, when the c-tor is called, "<<endl;
	cout<<"     the vptr is set to point to current's class vtable, so the c-tor of the current class couldn't be called via vptr, but the one of its "<<endl;
	cout<<"     Base class, as the Derived part had not been built yet. Steps executed by constructor:"<<endl;
	cout<<"     	1. call parent's class c-tor, to construct the Base part"<<endl;
	cout<<"     	2. sets vptr inherited from parent to point to current class vtable. Thus, virtual methods called within c-tor can resolve to current class versions."<<endl;
	cout<<"     	3. executes member initializer list"<<endl;
	cout<<"     	4. executes instructions specified in c-tors body implementation"<<endl<<endl;

	cout<<"     5. Destructors must be declared as virtual whean dealing with polymorphism. This was is ensured the resources pointed to by Base pointer"<<endl;
	cout<<"     or ref are properly deallocated."<<endl;
	cout<<"     Steps executed by destructor:"<<endl;
	cout<<"     	1. sets vptr inherited from parent to point to current caller class vtable. This means all virtual methods called in d-tor will call Base versions."<<endl;
	cout<<"     	2. executes instructions specified in d-tor body implementation"<<endl;
	cout<<"     	2. calls members' destructors to deallocate them."<<endl;
	cout<<"     	4. call parent's class d-tor, to destory the Base part"<<endl<<endl;

	cout<<"     6. The override qualifier can be specified after method's name, in Derived classes that override virtual methods inherited from Base class,"<<endl;
	cout<<"     in order to enforce a correct overriding, such that one of the four criteria above is not violated. Otheriwse, error is thrown at compile"<<endl;
	cout<<"     time. This way is ensured that when a Derived instance is passed to a Base ref/pointer which would call the virtual method, no ambiguous"<<endl;
	cout<<"     call or no matching call is issued, as the incompatiblity is detected when the class is implemented, not when the class is used."<<endl<<endl;

	cout<<"     7. The final qualifier can be specified after method's name, in Derived classes that override virtual methods inherited from Base class, "<<endl;
	cout<<"     in order to ensure that virtual method will not be overriden further, down in the inheritance chain."<<endl<<endl;

	cout<<"     8. Calling virtual methods in ctor/dtor should be avoided, otherwise, if such a call occurs in the most Base class, when Derived objects"<<endl;
	cout<<"     are constructed/destructed their overrides to the virtual method, if any, are not invoked. This happens as a consequence of object's build"<<endl;
	cout<<"     process that takes place in phases. When the c-tor of Derived is called, it calls Base ctor firstly, where the virtual method's call happens"<<endl;
	cout<<"		and is resolved to Base::virtualMethod() as the Derived part has not been built yet at this time. Nevertheless, if Derived ctor also"<<endl;
	cout<<"		invokes its override of the virtual method, this call would be resolved to Derived::virtualMethod(). "<<endl;
	cout<<"		Next to that, when such a call takes place in dtor, also due to the destruction process that takes place in phases, the call to the virtual"<<endl;
	cout<<"		method in Base dtor would be resolved to Base::virtualMethod() when a Derived is destroyed, as the call is made in the Base part. When the "<<endl;
	cout<<"		destruction process finishes with the Derived part and reaches the Base part, the vtable of the Derived part is not available anymore."<<endl;
	cout<<"		That said, the standard specifies that the runtime type of the calling object is that of the class being constructed/destructed."<<endl;

	cout<<"     9. Static methods can not be virtual, as static methods belong to class and cannot be bound to an object. Even if invoking a static method"<<endl;
	cout<<"     using a class instance  is an accepted syntax, it is counter intuitive. If it had been posisble to have static virtual methods, it would"<<endl;
	cout<<"     mean that a call to such method should access the vptr, in order to resolve the call. It is known that static methods can only work with"<<endl;
	cout<<"     static members, as they lack *this poiner. That said, a static virtual method would require a static vptr. But, if vptr would be static,"<<endl;
	cout<<"     it would mean that all classes in the inheritance chain will have the same vptr, thus the same vtable, is it would not properly implement"<<endl;
	cout<<"     the mechanisms used for dynamic dispatch in order to achieve polymorphic behavior."<<endl<<endl;

	cout<<"How virtuality works with multiple inheritance"<<endl;
	cout<<" 	1. Subsequently to multiple inheritance, a Derived instance can be pointed to/refered to by a pointer/reference to any if its Base classes. "<<endl;
	cout<<" 	Here, such a pointer/reference can access the virtual methods afferent to its Base class type. If any of these methods are overriden by"<<endl;
	cout<<" 	Derived, the overrides are invoked. In addition, upon assignment a static_cast to the appropriate Base is performed on Derived such that"<<endl;
    cout<<" 	the pointer/reference can access its corresponding Base part in the Derived's memory model. That said, the Base(n) reference to Derived"<<endl;
	cout<<" 	can only access methods from Base(n) part and from Derived part, not from Base(n-1) or Base(n+1)"<<endl; 

	cout<<" 	2. The virtual inheritance is used in case of multiple inheritance from (at least two) classes that share a common top base, known as diamond"<<endl;
	cout<<" 	problem. The class that multiple inherits, let's say Bottom, will have a part corresponding to each Base class, as it happens with regular"<<endl;
	cout<<" 	multiple inheritance. Netx to that, as at least 2 Base classes inherit from the same Top class, when a Bottom object is constructed, then"<<endl;
    cout<<" 	in the ctor/dtor call hierarchy the Top class would be constructed/destroyed twice (or more). This is not desirable, and is solved if the"<<endl;
	cout<<" 	Base classes do virtual inherit from Top class. In that way, at runtime, when the Bottom instance is built, it is checked whether the Top"<<endl;
	cout<<" 	part has already been constructed/destroyed."<<endl; 

	Derived d{};
	Base3& base3Ref{d};
	base3Ref.printName();
	base3Ref.printString();


	virtualityUnderTheHood();

	// Root* rPtr = new Base{};

	// auto vptrFunc = (void(*)(void))(*(int*)*(int*)rPtr);

	// cout<<"HERE"<<endl;
	// vptrFunc();

	return 0;
}
