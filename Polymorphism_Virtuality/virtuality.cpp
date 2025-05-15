#include "headers.hpp"

void virtualityUnderTheHood()
{
    using namespace std;

        cout<<"How virtuality works under the hood: Vptr and vtable"<<endl;
	cout<<"         1. A class that declares a virtual function, for the first time in the inheritance chain, gets a public non-static pointer referred to "<<endl;
	cout<<" 	as vptr, thus becoming larger by a pointer. That said, the vptr is allocated per object. Thereafter, the vptr points to a statically allocated array/struct"<<endl;
	cout<<" 	pf pointers to the functions declared as virtual. This array/struct is referred to as the vtable. As all class instances require access to the same "<<endl;
	cout<<" 	methods, the vtable is generated one per class at compile time, as it is known how many virtual methods a class has when it is compiled."<<endl;
	cout<<" 	When a class that derives from a Base that has at least one virtual method, it inherits the vptr and the vtable. If an inherited virtual "<<endl;
	cout<<" 	method is overriden in  the Derived class, its entry in the Derived class vtable is updated. Otherwise, the entries remain the same. Thus,"<<endl;
	cout<<" 	at runtime, when a virtual method is invoked,  the vptr is dereferenced to access the vtable and the apropriate entry for the virtual"<<endl;
    	cout<<" 	method is found and the call is resolved to it. The vtable can be perceived as a struct encapsulating raw pointers to virtual methods."<<endl<<endl; 

	struct VTable 
	{
	    void (*func1)();                   // Function with no parameters
	    int (*func2)(int);                 // Function taking int, returning int
	    void (*func3)(double, const char*); // Function taking double and const char*
	};

	cout<<" 	2. Late binding is used to resolve method call at runtime. Binding is the process that converts variables and functions into addresses. "<<endl;
	cout<<" 	For pointers, it always takes place at runtime, and is referred to as late/dynamic binding. So, even though the vtable is generated at "<<endl;
	cout<<" 	compile time, the addresses of the functions are only known at runtime."<<endl;
	cout<<" 	Concretely, at each compilation, a distinct address is generated in the process' virtual memory space for each function (generally speaking),"<<endl;
	cout<<" 	such that an entry in the vtable is assigned to a new value whenever the code is recompiled. Nevertheless, a mapping between each virtual"<<endl;
	cout<<" 	method's signature and an index in the vtable is done at compile time because it is known the number of virtual methods the given class "<<endl;
	cout<<" 	has (namely, the length of the statically allocated array of pointers to  functions), but it cannot be distinguished to which implementation"<<endl;
	cout<<" 	a mapping is done, because it is performed at runtime, via late binding. The index is fixed in the inheritance chain."<<endl<<endl;

	cout<<"         3. When a pointer/reference to Base class points/refers to a Derived object, it can only access the methods and members from the Base part"<<endl;
	cout<<" 	of the Derived object. In detail, it is known each class defines a namespace the encapsulation takes place within. If a class inherritance"<<endl;
	cout<<" 	tree is regarded as a nesting of namespaces, the outmost namespace, associated with the most Base class, cannot access data and functions"<<endl;
	cout<<" 	in the namespaces it nests. Moreover, the assignment of a Derived object to a Base pointer/reference is augmented by the compiler as a "<<endl;
	cout<<" 	static cast: Base* bPtr = static_cast<Base*>(new Derived()). "<<endl;
	cout<<" 	Next to that, it is also known that each method's name is mangled, at compile time, such that its first parameter is the *this pointer, which"<<endl;
	cout<<" 	is inferred from the calling object, so it is known for which object the given method is called and which values do the members bear in the"<<endl;
    	cout<<"     	invoked method. So it happens with virtual methods, only that the pointer/reference to Base class needs to be casted to the Derived"<<endl;
    	cout<<"     	pointer/reference."<<endl<<endl;

	cout<<"     	Having said that, given the vptr is placed in the public part, it can be directly accessed by the pointer/reference to Base. Next to that,"<<endl;
	cout<<"     	if the pointer/reference is used to invoke a virtual method, the call takes places via the vptr, which directly accesses the pointers to"<<endl;
	cout<<"     	the virtual methods (No pointer indirection vptr->vtable occurrs due to efficiency reasons). With the corresponding index for the given"<<endl;
	cout<<"     	method's signature being known, a virtual method call would look like below"<<endl; 
    	cout<<"     	Derived d; "<<endl;
	cout<<" 	Base& refBase{d);"<<endl;
	cout<<" 	refBase.virtualMethod()    ---> at compile time might look like:  (refBase.vptr[index])(static_cast<Derived&>(refBase))"<<endl;

	cout<<"Vtables for class hierarchy Root->Base->DerivedImpl obtained with 'g++ -fdump-lang-class file.cpp'"<<endl;
	cout<<"Class Root"<<endl;
   	cout<<"size=8 align=8"<<endl;
   	cout<<"base size=8 base align=8"<<endl;
	cout<<"Root (0x0x7f814a5319c0) 0 nearly-empty"<<endl;
    	cout<<"vptr=((& Root::_ZTV4Root) + 16)"<<endl;
	cout<<"Root::_ZTV4Root: 4 entries"<<endl;
	cout<<"0     (int (*)(...))0"<<endl;
	cout<<"8     (int (*)(...))(& _ZTI4Root)"<<endl;
	cout<<"16    0"<<endl;
	cout<<"24    0"<<endl;
	cout<<"32    (int (*)(...))__cxa_pure_virtual"<<endl;
	cout<<"40    (int (*)(...))Root::virtualMethod"<<endl;
	cout<<" "<<endl;

	cout<<"Class Base"<<endl;
   	cout<<"size=48 align=8"<<endl;
   	cout<<"base size=44 base align=8"<<endl;
	cout<<"Base (0x0x7f814a7f99c0) 0"<<endl;
    	cout<<"	vptr=((& Base::_ZTV4Base) + 16)"<<endl;
 	cout<<"		Root (0x0x7f814a574060) 0 nearly-empty"<<endl;
    	cout<<"			primary-for Base (0x0x7f814a7f99c0)"<<endl;
	cout<<"Base::_ZTV4Base: 4 entries"<<endl;
	cout<<"0     (int (*)(...))0"<<endl;
	cout<<"8     (int (*)(...))(& _ZTI4Base)"<<endl;
	cout<<"16    (int (*)(...))Base::~Base"<<endl;
	cout<<"24    (int (*)(...))Base::~Base"<<endl;
	cout<<"32    (int (*)(...))Base::pureVirtualMethod"<<endl;
	cout<<"40    (int (*)(...))Base::virtualMethod"<<endl;
	cout<<" "<<endl;

	cout<<"Class DerivedImpl"<<endl;
  	cout<<"size=56 align=8"<<endl;
   	cout<<"base size=56 base align=8"<<endl;
	cout<<"DerivedImpl (0x0x7f814a604b60) 0"<<endl;
    	cout<<"		vptr=((& DerivedImpl::_ZTV11DerivedImpl) + 16)"<<endl;
  	cout<<"	 Base (0x0x7f814a604bc8) 0"<<endl;
    	cout<<"			primary-for DerivedImpl (0x0x7f814a604b60)"<<endl;
    	cout<<"		Root (0x0x7f814a61df00) 0 nearly-empty"<<endl;
    	cout<<"				primary-for Base (0x0x7f814a604bc8)"<<endl;
	cout<<"DerivedImpl::_ZTV11DerivedImpl: 4 entries"<<endl;
	cout<<"0     (int (*)(...))0"<<endl;
	cout<<"8     (int (*)(...))(& _ZTI11DerivedImpl)"<<endl;
	cout<<"16    (int (*)(...))DerivedImpl::~DerivedImpl"<<endl;
	cout<<"24    (int (*)(...))DerivedImpl::~DerivedImpl"<<endl;
	cout<<"32    (int (*)(...))DerivedImpl::pureVirtualMethod"<<endl;
	cout<<"40    (int (*)(...))DerivedImpl::virtualMethod"<<endl;


	cout<<"     4. As it can be seen above, each vtable starts with an empty entry. It is called offset-to-top and is an entry in the table reserved."<<endl;
	cout<<"     	This entry is indexed with negative index -2, to store the offset to the corresponding Base class."<<endl;
	cout<<" 	In a liniar class hierarchy, that does not involve multiple inheritance, the offset is 0. (vtable[-2] = 0)"<<endl;
	cout<<" 	This offset is non zero for Derived classes that have multiple Base classes. (more details below)"<<endl;
	cout<<"     	The next entry is indexed with negative index -1. Hence, the pointers to virtual functions are indexed from 0 onwards."<<endl;
	cout<<" 	The second entry in vtable holds type info pointer to the RTTI: run-time type information, which is mainly accessed by 3 operators: type_info,"<<endl;
	cout<<" 	type_id, dynamic_cast."<<endl;
	cout<<" 	Then, if the dtor is virtual, it gets two entries. In an abstract class, its entries are 0, as the dtor does not require implementation,"<<endl;
	cout<<" 	because the class cannot be instantiated. In non-abstarct class, the 2 entries have the same signature. The first one is called complete"<<endl;
	cout<<" 	object dtor and is called for statically allocated objects' destruction, whereas the second one is called deleting destructor and it "<<endl;
	cout<<" 	performs delete on the dynamically allocated object, by internally invoking delete(). The selection of which dtor is called is done via a"<<endl;
	cout<<" 	hidden boolean parameter passed through a register, with the true value selecting the execution of the delete() operator."<<endl;
	cout<<" 	Thereafter, a pure virtual method is specially marked as pure_virtual as it is not invokable. In the classes that implement it, the entry"<<endl;
    	cout<<" 	is overriden with the method's signature."<<endl<<endl; 


	cout<<"How virtuality works under the hood: multiple inheritance"<<endl;
	cout<<"     1.  When it comes to multiple inheritance of a Derived class, the object's memory diagram layout firstly places the Base parts, in the"<<endl;
	cout<<" 	order specified at inheritance. That is Base1 part is followed by Base2 part and so on, with the last part corresponding to Derived. "<<endl;
	cout<<" 	If the Base classes contain virtual methods, each will have a vptr that will be inherited by Derived, thus the Derived object's memory "<<endl;
	cout<<" 	layout will contain a vptr for each Base part. This means that Derived will have multiple vtables and if it overrides a method inherited"<<endl;
	cout<<" 	from a Base parent, the vtable inherited from that Base will be updated with the considered override."<<endl;

	cout<<"     2.  In case of non-virtual multiple inheritance, the vtable of the Derived class firstly contains the vtable of the first class specified"<<endl;
	cout<<" 	in the derivation list, whose entries are updated with Derived overrides, if any. Furthermore, if Derived overrides virtual methods inherited"<<endl;
	cout<<" 	from the other Base classes, those entries are added to this first table, thus enlarging the vtable inherited from the first Base class."<<endl;
	
	cout<<" 	Next, the vtables inherited from other Base classes, are placed, in order. If Derived overrides any virtual method inherited from those"<<endl;
	cout<<" 	the corresponding entry in the vtable is updated with the override. If all Base classes have a virtual method with similar name, in the"<<endl;
	cout<<" 	vtables of the next Base classes (all, but first), the entries of this virtual method are populated with the address of the override from"<<endl;
	cout<<" 	the vtable corresponding to first Base class. This is normal, as there is only one implementation, and all vtables practically share it."<<endl;
	cout<<" 	Similarily it happens with the virtual dtor: both entries for it are added ot the first table, regardless the first Base class has a virtual"<<endl;
	cout<<" 	dtor or not, as it would suffice for at least one of the Base classes to define a virtual dtor. Then, in the vtable part corresponding"<<endl;
	cout<<" 	to the Base class(es) with virtual dtor, two extra entries are added, each containing offset to the dtor entry from the first Base vtable."<<endl;

	cout<<" 	Each subsequent vtable to the first one contains a value for the offset to the top. This value is used to compute the addresses of methods"<<endl;
	cout<<" 	shared by the vtables in the memory layout of Derived and is a negative integer. Also, this negative index represents the amount of bytes"<<endl;
	cout<<" 	that need to be shifted in order to point to the start of the object in memory (the first vtable), hence the name offset to top."<<endl;
	cout<<" 	Furthermore, this value is used when performing upcasting from pointer/ref to Derived to pointer/ref to one of the Base classes, but first,"<<endl;
	cout<<" 	followed by a downcast, via dynamic_cast. The downcasting actuallly uses the negative offset in order to compute the pointer/reference to"<<endl;
	cout<<" 	the start of the object, from the pointer/reference tot he given Base class."<<endl;


	Derived* dPtr = new Derived{};
	cout<<dPtr->euler<<endl;
	Derived derivedInst{};
	//the line below is augmented as static_cast<Base3&>(derived), thus an upcast occurs, to a reference to a base class,
	//with the reference now pointing to the Base3 part of the Derived's vtable
	Base3& base3Ref{derivedInst};
	//after dynamic_cast, the downCast reference will point to the top of the object's memory layout
	Derived& downCast = dynamic_cast<Derived&>(base3Ref);

	cout<<"Vtable for Base1"<<endl;
	cout<<" 	Base1::_ZTV5Base1: 6 entries"<<endl;
	cout<<" 	0     (int (*)(...))0"<<endl;
	cout<<" 	8     (int (*)(...))(& _ZTI5Base1)"<<endl;
	cout<<" 	16    (int (*)(...))Base1::~Base1"<<endl;
	cout<<" 	24    (int (*)(...))Base1::~Base1"<<endl;
	cout<<" 	32    (int (*)(...))Base1::printName"<<endl;
	cout<<" 	40    (int (*)(...))Base1::printInt"<<endl;
	cout<<" "<<endl;

	cout<<"Vtable for Base2"<<endl;
	cout<<" 	Base2::_ZTV5Base2: 4 entries"<<endl;
	cout<<" 	0     (int (*)(...))0"<<endl;
	cout<<" 	8     (int (*)(...))(& _ZTI5Base2)"<<endl;
	cout<<" 	16    (int (*)(...))Base2::printName"<<endl;
	cout<<" 	24    (int (*)(...))Base2::printEuler"<<endl;
	cout<<" "<<endl;

	cout<<" 	Vtable for Base3"<<endl;
	cout<<" 	Base3::_ZTV5Base3: 4 entries"<<endl;
	cout<<" 	0     (int (*)(...))0"<<endl;
	cout<<" 	8     (int (*)(...))(& _ZTI5Base3)"<<endl;
	cout<<" 	16    (int (*)(...))Base3::~Base3"<<endl;
	cout<<" 	24    (int (*)(...))Base3::~Base3"<<endl;
	cout<<" 	32    (int (*)(...))Base3::printName"<<endl;
	cout<<" 	40    (int (*)(...))Base3::printChar"<<endl;
	cout<<" "<<endl;

	cout<<" 	Vtable for Derived"<<endl;
	cout<<" 	Derived::_ZTV7Derived: 15 entries"<<endl;
	cout<<" 	0     (int (*)(...))0"<<endl;
	cout<<" 	8     (int (*)(...))(& _ZTI7Derived)"<<endl;
	cout<<" 	16    (int (*)(...))Derived::~Derived"<<endl;
	cout<<" 	24    (int (*)(...))Derived::~Derived"<<endl;
	cout<<" 	32    (int (*)(...))Derived::printName"<<endl;
	cout<<" 	40    (int (*)(...))Base1::printInt"<<endl;
	cout<<" 	48    (int (*)(...))Derived::printEuler"<<endl;
	cout<<" 	56    (int (*)(...))Derived::printString"<<endl;
	cout<<" 	64    (int (*)(...))-16"<<endl;
	cout<<" 	72    (int (*)(...))(& _ZTI7Derived)"<<endl;
	cout<<" 	80    (int (*)(...))Derived::_ZThn16_N7Derived9printNameEv"<<endl;
	cout<<" 	88    (int (*)(...))Derived::_ZThn16_N7Derived10printEulerEv"<<endl;
	cout<<" 	96    (int (*)(...))-32"<<endl;
	cout<<" 	104    (int (*)(...))(& _ZTI7Derived)"<<endl;
	cout<<" 	112   (int (*)(...))Derived::_ZThn32_N7DerivedD1Ev"<<endl;
	cout<<" 	120   (int (*)(...))Derived::_ZThn32_N7DerivedD0Ev"<<endl;
	cout<<" 	128   (int (*)(...))Derived::_ZThn32_N7Derived9printNameEv"<<endl;
	cout<<" 	136   (int (*)(...))Base3::printChar"<<endl;

	cout<<" 	Vtable for Bottom"<<endl;
	cout<<" 	Bottom::_ZTV6Bottom: 24 entries"<<endl;
	cout<<" 	0     64"<<endl;
	cout<<" 	8     (int (*)(...))0"<<endl;
	cout<<" 	16    (int (*)(...))(& _ZTI6Bottom)"<<endl;
	cout<<" 	24    (int (*)(...))Bottom::printName"<<endl;
	cout<<" 	32    (int (*)(...))Left::printEuler"<<endl;
	cout<<" 	40    (int (*)(...))Bottom::~Bottom"<<endl;
	cout<<" 	48    (int (*)(...))Bottom::~Bottom"<<endl;
	cout<<" 	56    (int (*)(...))Bottom::printString"<<endl;
	cout<<" 	64    48"<<endl;
	cout<<" 	72    (int (*)(...))-16"<<endl;
	cout<<" 	80    (int (*)(...))(& _ZTI6Bottom)"<<endl;
	cout<<" 	88    (int (*)(...))Bottom::_ZThn16_N6Bottom9printNameEv"<<endl;
	cout<<" 	96    (int (*)(...))Right::printChar"<<endl;
	cout<<" 	104   (int (*)(...))Bottom::_ZThn16_N6BottomD1Ev"<<endl;
	cout<<" 	112   (int (*)(...))Bottom::_ZThn16_N6BottomD0Ev"<<endl;
	cout<<" 	120   0"<<endl;
	cout<<" 	128   0"<<endl;
	cout<<" 	136   18446744073709551552"<<endl;
	cout<<" 	144   18446744073709551552"<<endl;
	cout<<" 	152   (int (*)(...))-64"<<endl;
	cout<<" 	160   (int (*)(...))(& _ZTI6Bottom)"<<endl;
	cout<<" 	168   (int (*)(...))Bottom::_ZTv0_n24_N6BottomD1Ev"<<endl;
	cout<<" 	176   (int (*)(...))Bottom::_ZTv0_n24_N6BottomD0Ev"<<endl;
	cout<<" 	184   (int (*)(...))Bottom::_ZTv0_n32_N6Bottom9printNameEv"<<endl;
	cout<<" 	192   (int (*)(...))Top::printInt"<<endl;
	cout<<" 	200   (int (*)(...))TopRoot::doNothing"<<endl;

	cout<<"     3.  A class that has multiple Base classes which virtually inherit froma common top base class, has its object memory split into 2 regions:"<<endl;
	cout<<" 	invariant and variant. In the first section reside the parts that have fixed offsets, which correspond to Bottom and its direct Base classes."<<endl;
	cout<<" 	In the variants section reside Top base classes from which at least two Base classes inherit (that are virtual Base classes). This section"<<endl;
	cout<<" 	is placed after the invariant one, including in the vtable layout. In the vtable, this region contains virtual methods inherited from Top class"<<endl;
	cout<<" 	but not subsequently overriden. That said, as it happens with regular multiple inheritance, the first vptr and vtable of Bottom correspond"<<endl;
	cout<<" 	to the first Base class."<<endl;

	cout<<"     The vtable of the first Base class of Bottom can be used to access the data members of Top virtual base class, using negative indexing."<<endl;
	cout<<" 	invariant and variant. In the first section reside the parts that have fixed offsets, which correspond to Bottom and its direct Base classes."<<endl;
	cout<<" 	In the variant section reside Top base classes from which at least two Base classes inherit (that are virtual Base classes). This section"<<endl;
	cout<<" 	is placed after the invariant one, including in the vtable layout. In the vtable, this region contains virtual methods inherited from Top class"<<endl;
	cout<<" 	but not subsequently overriden."<<endl;

	cout<<"     When it comes to the virtual dtor inherited from Top class, in the Bottom object memory diagram, the 2 entries are generated in the vtable"<<endl;
	cout<<" 	corresponding to the first Base class. The next vtables, including the one corresponding to Top class, contain the 2 entries for the virtual"<<endl;
	cout<<" 	dtor, with the afferent values being offsets to the entries of dtors from the vtable of the first Base class."<<endl;

}
