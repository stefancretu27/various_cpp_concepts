#include "VirtualInheritance.hpp"
#include "MultipleInheritance.hpp"

#include <set>
#include <unordered_set>
#include <vector>

using namespace std;

int main()
{
    cout<<"Insights on dynamic_cast"<<endl;
    cout<<"     1. Converts pointer and references up, down and sideways along the inheritance hierarchy. The classes must be polymorphic types, that is "<<endl;
    cout<<"     they must have a virtual method, at least. Therefore, a vtable exists wihtin the memory layout, which contains the offset to top that is used"<<endl;
    cout<<"     by dynamic_cast to set the pointer to Base of a Derived object to the top of the memory layout of Derived. That is because after un upcast"<<endl;
    cout<<"     from Derived to Base (that can very well be static_cast) a pointer/reference to that Base part of Derived is returned. That said, the"<<endl;
    cout<<"     returned pointer/reference will point/refer to another chunk in the memory model of Derived, which is found at top + an offset."<<endl<<endl;
    
    cout<<"     2. In case of upcast, it can be also done with static_cast. An upcast is always valid with both static and dynamic cast, but since"<<endl;
    cout<<"     it is an implicit conversion, it can take place via copy constructing or copy assigning pointers or references."<<endl<<endl;

    cout<<"     3. Dynamic_cast involves a run time check. This is done in order to properly find the appropriate resolution for downcast and sidecast"<<endl;
    cout<<"     leveraging typeinfo section from vtable, indexed with -1. The RTTI (run time type identification) process can be disabled from compiler."<<endl;
    cout<<"     Thus, at runtime, the inheritance tree is traversed, to check for type_info in vtable[-1], but this check can only be done for public."<<endl;
    cout<<"     inheritance and will fail for protected and private inheritance."<<endl;
    
    cout<<"     4. Downcasts can be performed with static_cast, as well, and it is safe only when the convert from pointer/ref that points to Derived."<<endl;
    cout<<"     When downcasting to Bottom from a Top reference that indicates to Left or Right, the returned reference is invalid, as it cannot be null."<<endl;
    cout<<"     Thus, it is recommended to perform dynamic_casts with references in try/catch blocks, in order to catch bad_cast exceptions."<<endl<<endl;
   
    cout<<"     5. Dynamic_cast is useful when it is not known the runtime object pointed/referred to by the argument. Downcasting with static_cast is also"<<endl;
    cout<<"     possible when it is known the type of the object pointed/referred to. Additionally, if the cast between references fails, dynamic_cast"<<endl;
    cout<<"     throws exception whereas static_cast lead to undefined behavior. Also, dynamic_cast is used to convert from virtual Base classes."<<endl<<endl;
    
    
    cout<<"upcasts and downcasts within virtual inheritance hierarchy"<<endl;
    cout<<"     upcast from Bottom to Left"<<endl;
    //upcast example using implicit conversion
    Bottom bottomInst{};
    Left& upcastToLeft {bottomInst};
    //overriden method in all inheritance chain
    upcastToLeft.printName();
    //method define din Top class
    upcastToLeft.printInt();
    //method defined in Left, overriden in Bottom
    upcastToLeft.printEuler();
    cout<<"     downcast with static_cast from Left back to Bottom"<<endl;
    //downcast from the upcasted reference above. Since upcastToLeft refers to a Bottom object, it is safe to use static_cast
    Bottom& downcastToBottom = static_cast<Bottom&>(upcastToLeft);
    downcastToBottom.printName();
    downcastToBottom.printEuler();
    downcastToBottom.printString();

    cout<<" static downcast from Left instance To Bottom. Left is not virtual base for Bottom and "<<endl;
    Left leftObj{};
    Bottom& downcastFromLeftInst = static_cast<Bottom&>(leftObj);
    downcastFromLeftInst.printName();

    cout<<"     upcast from Bottom to Right"<<endl;
    Right& upcastToRight{bottomInst};
    upcastToRight.printName();
    upcastToRight.printString();
    upcastToRight.printInt();
    upcastToRight.printChar();
    cout<<"     downcast with static_cast from Right back to Bottom"<<endl;
    Bottom& rightToBottom = static_cast<Bottom&>(upcastToRight);
    rightToBottom.printName();
    rightToBottom.printString();
    rightToBottom.printInt();
    rightToBottom.printChar();

    cout<<"     upcast from Bottom to Top"<<endl;
    Top& upcastToTop{bottomInst};
    upcastToTop.printName();
    upcastToTop.printInt();
    cout<<"     downcast with static_cast from Top back to Bottom: cannot convert pointer to base class Top to pointer to derived class Bottom -- base class is virtual"<<endl;
    //Bottom& topToBottom = static_cast<Bottom&>(upcastToTop);
    
    cout<<"casting from virtual Base to derived types is done exclusively via dynamic_cast"<<endl;
    Left leftInst{};
    Top& refTop{leftInst};
    Left& refLeft = dynamic_cast<Left&>(refTop);
    refLeft.printName();
    refLeft.printEuler();

    
    
    try
    {
        Right& downcastToRight = dynamic_cast<Right&>(refTop);
        downcastToRight.printName();
        downcastToRight.printString();
        downcastToRight.printInt();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << endl;
    }

    //downcast to non existing Bottom object is impossible with static cast
    Left left;
    Top& leftToTop{left};
    try
    {
        Bottom& downcastFromTopLeft = dynamic_cast<Bottom&>(leftToTop);
        downcastFromTopLeft.printName();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << endl;
    }

    //sidecast from the Left reference to Right reference is possible only with dynamic cast
    cout<<"     sidecast only with dynamic_cast"<<endl;
    try
    {
        Right& sidecastRight = dynamic_cast<Right&>(upcastToLeft);
        sidecastRight.printName();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << endl;
    }

    cout<<"Insights on static_cast"<<endl;
    cout<<"     1. Performs conversion between related types. Upcasts can be performed, though they are implicit."<<endl;
    cout<<"     Also, downcasts can be performed, as long as inthe inheritance chain is not involved a virtual base class. If a downcast to lower"<<endl;
    cout<<"     level is requested, from a Top reference to an intermediate level, the intermediate type is returned."<<endl;

    cout<<"     2. There is a performance advantage when downcasting with static_cast, as it does not entail runtime type check."<<endl<<endl;

    cout<<"     3.A static-cast from a lvalue to && is equivalent to move: it moves ownership to the object returned by the cast operation."<<endl<<endl;

    MoreDerived moreDerivedInst{};
    Base2& moreDerivedToBase2{moreDerivedInst};
    moreDerivedToBase2.printName();
    MoreDerived& refMoreDerived = static_cast<MoreDerived&>(moreDerivedToBase2);
    refMoreDerived.printName();

    Derived derivedInst{};
    Base3& derivedToBase3{derivedInst};
    derivedToBase3.printName();
    //conversion will return a Derived instance
    EvenMoreDerived& refToEvenMoreDerived = static_cast<EvenMoreDerived&>(derivedToBase3);
    refToEvenMoreDerived.printName();

    //conversion will return bad_cast exception as a EvenMoreDerived part does not exist in the Derived object refered to by Base3&
    try
    {
        EvenMoreDerived& anotherRefToEvenMoreDerived = dynamic_cast<EvenMoreDerived&>(derivedToBase3);
        anotherRefToEvenMoreDerived.printName();
    }
    catch(const exception& e)
    {
        std::cerr<<e.what()<<std::endl;
    }

    int i{8};
    float f = static_cast<float>(i);
    cout<<f<<endl;
    double d{3.14159};
    int ii(d);
    cout<<ii<<endl;
    float* pf = new float{2.7182};
    int* pi = reinterpret_cast<int*>(pf);
    cout<<*pf<<" "<<*pi<<endl;

    delete pi;


    set<int> setInt;
    setInt.emplace(7);
    setInt.insert({6, 5, 9, 19, -3});

    set<int> vi = static_cast<set<int>&&>(setInt);
    cout<<setInt.size()<<" "<<vi.size()<<endl;

    return 0;
}