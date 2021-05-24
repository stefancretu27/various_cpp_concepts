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
	cout<<endl<<"Insights on inheritance"<<endl;
    cout<<"     1. Derived class does not inherit friend functions and classes of the Base class. Therefore, there is impossible to override a friend function"<<endl<<endl;
    cout<<"     2. A Derived class inherits the following from the Base class:"<<endl;
    cout<<"     	- Constructors, including Copy C-tor"<<endl;
	cout<<"     	- Destructor"<<endl;
	cout<<"     	- Copy assignment operator"<<endl;
	cout<<"     	- all public and protected methods"<<endl;
	cout<<"     	- all members. The private ones are only accessible via setters and getters, whiel the protected and public ones are accessible in Derived scope."<<endl<<endl;
	
	cout<<"     3. Derived class's implicitly generated copy semantics automatically calls the Base class copy semantics, regardless the Base, class implements it or uses the default"<<endl;
	cout<<"		one. The default Derived c-tor automatically calls deafult Base c-tor, if no explicit call to a Base c-tor overload is specified. This happens as the Derived object"<<endl;
	cout<<"		is built in phases: first the Base part, then the Derived part."<<endl<<endl;
	
	cout<<"		4. Derived class d-tor automatically calls the Base d-tor, as the destruction also takes place in phases: first is destroyed the Derived part, then the Base part."<<endl<<endl;
	
	cout<<"     5. Derived class own implemented c-tor and copy c-tor automatically call Base class default c-tor, as a new object must be created. However, they can explicitly "<<endl;
	cout<<"		call, in their member initializer list, the c-tors which are implemented in Base class, as Derived implemented c-tors do not automatically call c-tors overloads from Base."<<endl<<endl;
	cout<<"     6. Derived implemented copy assignment has to explicitly invoke Base copy assignment, otherwise none will be called. Also, default c-tor of Base class is not"<<endl;
	cout<<"		called, as the object assigned to has already been constructed."<<endl<<endl;
	
	cout<<"     7. Constructors, including copy c-tor, can use member initializer list for initializing const and reference members. However, a Derived class can only initialize"<<endl;
	cout<<"     its members in its initializer list, but not the members inherited from the Base class. This makes sense as Derived c-tor automatically calls Base implicit c-tor"<<endl;
	cout<<"     which performs initialization on Base members. If Derived would have initialized Base members in Derived member initializer list, this would lead to double"<<endl; 
	cout<<"     initialization, which would be an error for const and references data. However, Derived c-tors can invoke, in their member initializer list, appropriate Base c-tors to perform"<<endl;
	cout<<"     initialization of members inherited from Base."<<endl<<endl;
	
	cout<<"     8. Override: in Derived class, for the methods from the Base class, the signature stays the same (name, number and types of parameters and the return type)"<<endl;
	cout<<"     but the implementation can be changed. However, the method from the Base class can still be called by Derived object using Base:: scope resolution."<<endl<<endl;
	
	cout<<"     9. Shadowing: in Derived class is changed the parameter list for a method inherited from the Base class. In other words, it looks like mixing overriding and overloading."<<endl;
	cout<<"     However, the method from the Base class can still be called by Derived object using Base:: scope resolution."<<endl<<endl;


	cout<<endl<<"Create DerivedImpl instance"<<endl;
	DerivedImpl derivedImpl_Inst{"derived class implementing rule of 5", 2.71};
	cout<<"Label: "<<derivedImpl_Inst.getLabel()<<" | data: "<<derivedImpl_Inst.getData()<<endl<<endl;

	DerivedImpl derivedInst{"derived class built without explicit Base c-tor call", 2.71, true};
	cout<<"ID: "<<derivedInst.getID()<<"Label: "<<derivedInst.getLabel()<<" | data: "<<derivedInst.getData()<<endl;

	cout<<endl<<"Create copy of DerivedImpl instance using Derived implicitly generated copy c-tor"<<endl;
	DerivedImpl copyDefault_derivedImpl_Inst{derivedImpl_Inst};
	cout<<"Original instance Label: "<<derivedImpl_Inst.getLabel()<<" | data: "<<derivedImpl_Inst.getData()<<endl;
	cout<<"Copy instance Label: "<<copyDefault_derivedImpl_Inst.getLabel()<<" | data: "<<copyDefault_derivedImpl_Inst.getData()<<endl;

	cout<<endl<<"Create copy of DerivedImpl instance using Derived implemented copy c-tor, that does not call Base copy c-tor"<<endl;
	DerivedImpl copy_derivedImpl_Inst{derivedImpl_Inst, 1};
	cout<<"Original instance Label: "<<derivedImpl_Inst.getLabel()<<" | data: "<<derivedImpl_Inst.getData()<<endl;
	cout<<"Copy instance Label: "<<copy_derivedImpl_Inst.getLabel()<<" | data: "<<copy_derivedImpl_Inst.getData()<<endl;

	cout<<endl<<"Create copy of DerivedImpl instance using Derived implemented copy c-tor, that explicitly calls Base copy c-tor"<<endl;
	DerivedImpl copyBase_derivedImpl_Inst{derivedImpl_Inst, true};
	cout<<"Original instance Label: "<<derivedImpl_Inst.getLabel()<<" | data: "<<derivedImpl_Inst.getData()<<endl;
	cout<<"Copy instance Label: "<<copyBase_derivedImpl_Inst.getLabel()<<" | data: "<<copyBase_derivedImpl_Inst.getData()<<endl;

	cout<<endl<<"Create move copy of DerivedImpl instance"<<endl;
	DerivedImpl move_derivedImpl_Inst{move(derivedImpl_Inst)};
	cout<<"Original instance Label: "<<derivedImpl_Inst.getLabel()<<" | data: "<<derivedImpl_Inst.getData()<<endl;
	cout<<"Move instance Label: "<<move_derivedImpl_Inst.getLabel()<<" | data: "<<move_derivedImpl_Inst.getData()<<endl;

	cout<<endl<<"Copy assign to original instance, that had been previously moved"<<endl;
	derivedImpl_Inst = copyBase_derivedImpl_Inst;
	cout<<"Original instance Label: "<<derivedImpl_Inst.getLabel()<<" | data: "<<derivedImpl_Inst.getData()<<endl;

	cout<<endl<<"Move assign to original instance, that had been previously moved"<<endl;
	DerivedImpl moveAssign_derivedImpl_Inst;
	moveAssign_derivedImpl_Inst = move(derivedImpl_Inst);

	auto values = moveAssign_derivedImpl_Inst.getAttributes();
	cout<<get<0>(values)<<" "<<get<1>(values)<<" "<<get<2>(values)<<endl;
	auto val = moveAssign_derivedImpl_Inst.Base::getAttributes();
	cout<<get<0>(val)<<" "<<get<1>(val)<<endl;


	cout<<endl<<"Conclusions on Derived class inheriting from Base class implemented based on rule of 5"<<endl;
    cout<<"     1. A std::move call on a Derived object firstly looks in the Derived class to find appropriate move c-tor or move assignment. If not found, then it looks for"<<endl;
	cout<<"     copy c-tor or copy assignment in Derived class. If also not found, then it move up in the inheritance chain to looks for move semantics, then for copy semantics"<<endl<<endl;
	
	cout<<"     2. If the Derived class follows rule of 0, it inherits all copy and move semantics from the Base class, that follows rule of 5. Only that the copy and the move"<<endl;
    cout<<"     operations are performed between Base parts of the Derived objects. "<<endl<<endl;
    
	cout<<"     3. Move semantics implemented in Derived class must explicitly invoke move semantics from Base class, in its implementation. Otherwise, Base class default c-tor is "<<endl;
	cout<<"		called instead. In other words, move c-tor and move assignment operator from Base class are not implicitly called by move c-tor and move assignment operator in  "<<endl;
	cout<<"     Derived class, respectively"<<endl<<endl;
	
	cout<<"     4. It is known that implicitly generated copy semantics performs shallow copy. If the Derived class uses default copy semantics, besides performing shallow copy "<<endl;
	cout<<"     between Derived's members, it will automatically call copy semantics from Base class, regardless the Base class implements copy semantics or uses implicitly generated one."<<endl;

	return 0;
}
