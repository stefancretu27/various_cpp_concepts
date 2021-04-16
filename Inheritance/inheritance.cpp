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
	cout<<endl<<"Create DerivedImpl instance"<<endl;
	DerivedImpl derivedImpl_Inst{"derived class implementing rule of 5", 2.71};
	cout<<"Label: "<<derivedImpl_Inst.getLabel()<<" | data: "<<derivedImpl_Inst.getData()<<endl;

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

	cout<<endl<<"Copy assign to original instance, that had been previously moved"<<endl;
	DerivedImpl moveAssign_derivedImpl_Inst;
	moveAssign_derivedImpl_Inst = move(derivedImpl_Inst);


	cout<<endl<<"Conclusions on Derived class inheriting from Base class implemented based on rule of 5"<<endl;
    cout<<"     1. It is known that move c-tor and move assignment operator are not implicitly generated. If the Derived class does not implement them, but follows rule of 3,"<<endl;
    cout<<"     a move call to Derived class will call copy c-tor or copy assignment of Derived class. That said, rule of 3 Derived does not inherit move semantics from rule"<<endl;
    cout<<"     of 5 Base class. It does makes sense, as copy c-tor and copy assignment accept as argument references to r-value (that is treated as const) and can perform"<<endl;
	cout<<"     a proper memberwise copy or assignment for Derived objects, as both Base and Derived parts are taken into account."<<endl<<endl;
	cout<<"     2. If the Derived class follows rule of 0, it inherits all copy and move semantics from the Base class, that follows rule of 5. Only that the copy and the move"<<endl;
    cout<<"     operations are performed between Base parts of the Derived objects. "<<endl<<endl;
	cout<<"     3. Move semantics implemented in Derived class must explicitly invoke move semantics from Base class, in its implementation. In other words, move c-tor and move"<<endl;
	cout<<"     assignment operator from Base class are not implicitly called by move c-tor and move assignment operator in derived class, respectively"<<endl<<endl;
	cout<<"     4. Copy semantics implemented in Derived class must explicitly invoke copy semantics from Base class, in its implementation. In other words, copy c-tor and copy"<<endl;
	cout<<"     assignment operator from Base class are not implicitly called by copy c-tor and copy assignment operator in Derived class, respectively. This makes sense as,"<<endl;
	cout<<"     if one choses to implement copy and/or move semantics, he has to specify how these operations shall be performed between Base parts of the objects."<<endl;
	cout<<"     Explicitly invoke Base class copy/move semantics is one way of doing this. Also, reuse the code from Base class copy/move semantics in Derived copy/move semantics"<<endl;
	cout<<"     is another way of doing this, but it leads to code duplication/redundancy between Derived and Base."<<endl<<endl;
	cout<<"     5. It is known that implicitly generated copy semantics perform shallow copy. If the Derived class uses default copy semantics, besides performing shallow copy "<<endl;
	cout<<"     between Derived's members, it will automatically call copy semantics from Base class, regardless the Base class implements copy semantics or uses implicitly generated one."<<endl;
	
	return 0;
}
