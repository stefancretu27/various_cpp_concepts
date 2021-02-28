#include  "includes.hpp"
#include "HelperClass.hpp"

void ReferencesAndConst()
{
    using namespace std;

    float floatVal{0.05f}, pi{3.14159};
    const float constFloatVal{2.7182};
    float& ref_Float{floatVal};
    ref_Float = pi; //it is like performing floatVal = pi;
    cout<<"Assign a new value: "<<pi<<"to a reference => only updates the value of the referred object. Ref: "<<ref_Float<<" Aliased obj: "<<floatVal<<endl;
    ref_Float += 1.1f;
    cout<<"Update the value via the ref. Ref: "<<ref_Float<<" Aliased obj: "<<floatVal<<" previously <<assigned>> object"<<pi<<endl;
    float& ref_FloatFromRef{ref_Float};
    cout<<"ref: "<<ref_FloatFromRef<<" created from ref: "<<ref_Float<<endl;


    cout<<endl<<"Conclusions on references: "<<endl;
    cout<<"     1. References act like aliases to ojects. They must have the same type as the aliased variable, otherwise error is thrown."<<endl;
    cout<<"     2. References must be initialized upon creation, otherwise error is thrown. Pointers can be initialized any time."<<endl;
    cout<<"     3. References cannot be NULL. Pointers can be NULL."<<endl;
    cout<<"     4. References to non-const can only alias non-const l-values."<<endl;
    cout<<"     5. References can be copy constructed from other references, only that const variables must be aliased by const refs."<<endl;
    cout<<"     6. Once set to refer to an object, they cannot be reassigned to point to other object.Thus, they act like a const pointer. \
                That said, when assigning a new object to a reference, it actually updates the value of the refered to variable, not the reference itself."<<endl<<endl;

    //----------------------------------------------------------------------------------------------------------------------------------------
    //cannot bind non-const lvalue reference of type ‘double&’ to an rvalue of type ‘double’, as a r-value seems to be constructed from 'pi'
    //double& dRef{pi}; 
    const short int& ref_ConstShort{pi}; // => warning thrown, but not error
    cout<<"float value: "<<pi<<" short int reference: "<<ref_ConstShort<<endl;
    //attempt to change non const value referred by ref to const => error: assignment of read-only reference
    //ref_ConstShort = -12; 
    //cout<<"shoart value: -12 short int reference: "<<ref_ConstShort<<endl;
    
    //error const qualifier discared =>const reference to be used
    //float& ref_Float2{e}; 
    const float& ref_ConstFloat{constFloatVal}; //alias const l-value
    
    //error: reference to const so the value cannot be modified
    //ref_ConstFloat = pi; 
    const char& ref_ConstChar{'q'}; //alias r-value
    //attempt to update the value via ref to const char
    //ref_ConstChar = 'x'; => rvalue cannot be modified when referred to by ref to const. It requires ref to rvalue so it can be changed
    const char& ref_ConstChar2{ref_ConstChar};

    cout<<endl<<"Conclusions for references to const: "<<endl;
    cout<<"     1. References to const can alias const and non-const l-values, but also r-values. "<<endl;
    cout<<"     2. References to const treat the aliased pbject as const => cannot be changed via the reference. They are like const ptr to const."<<endl;
    cout<<"     3. References to const can alias objects of other types than the reference, as from the referenced object is built a r-value of the new type."<<endl;
    cout<<"     4. Used as function parameter type, prevent pass-by-value, allow r-value arguments, but the argument is const => non-modifiable"<<endl<<endl;

    //----------------------------------------------------------------------------------------------------------------------------------------
    //construct rvalue ref from lavlue => errorcannot bind rvalue reference of type ‘float&&’ to lvalue of type ‘float’ => need move semantics
    //float&& refRVal_Float{pi};
    float&& refRVal_Float{move(pi)};
    cout<<"rvalue reference obtained from lvalue float using move => ownership transfered to the ref to rvalue: "<<refRVal_Float<<endl;
    refRVal_Float += 11.11;
    cout<<"updated value via rvalue reference tobtained from lvalue float using move:: "<<refRVal_Float<<" original value not preserved: "<<pi<<endl;

    //rvalue reference from literal
    string&& refRVal_String{"rvalue string"};
    cout<<"rvalue reference to string literal: "<<refRVal_String<<endl;
    refRVal_String += " + added str";
    cout<<" updated string via rvalue ref: "<<refRVal_String<<endl;

    //rvalue reference to anonymous object
    HelperClass&& refRVal_Inst{HelperClass(1.1, 'k')};
    cout<<"rvalue ref to anonymous object: "<<refRVal_Inst.getD()<<" "<<refRVal_Inst.getC()<<endl;
    refRVal_Inst.setD(-273);
    cout<<"updated double value using rvalue to anonymous object as instance: "<<refRVal_Inst.getD()<<endl;

    //rvalue ref to temporary objects
    refRVal_Float = floatVal+15;
    cout<<"float val: "<<floatVal<<" ref to rvalue float val: "<<refRVal_Float<<endl;

    double&& refRVal_Double{refRVal_Inst.getD()};
    cout<<"ref to rvalue double initialized from value returned from function/method: "<<refRVal_Double<<endl;
    refRVal_Double += 28.28;
    cout<<"new referenced value: "<<refRVal_Double<<" initial referenced value "<<refRVal_Inst.getD()<<endl;

    shared_ptr<HelperClass>&& refRValue_SPtr{make_shared<HelperClass>(-22.7f, 'h')};
    cout<<"ref to rvalue shared ptr obtained from make_shared: "<<refRValue_SPtr->getD()<<" "<<refRValue_SPtr->getC()<<endl;

    cout<<endl<<"Conclusions for references to rvalues: "<<endl;
    cout<<"     1. References to rvalues can alias only r-values."<<endl;
    cout<<"     2. Rvalues can be obtained from lvalue using std::move = It is exactly equivalent to a static_cast to an rvalue reference type."<<endl;
    cout<<"        It also indicates that an object t may be <<moved from>>"<<endl;
    cout<<"     2. Rvalues are: anonymous objects, literals, temporary objects = such as i+=1, or objects returned from functions, like getters or make_shared"<<endl;
    cout<<"     3. Unlike references to const, they allow the referenced value to be modified."<<endl<<endl;

    //-----------------------------------------------------------------------------------------------------------------------------
    //more on std::move
    string myString{"test string"};
    //myString content is moved from myString to newString, so myString will be empty
    string newString{move(myString)};
    newString += " concatenated part";
    cout<<"original string: "<<myString<<" modified new string: "<<newString<<endl;

    double myDouble{-13.14};
    double newDouble{move(myDouble)};
    cout<<myDouble<<" "<<newDouble<<endl;

    set<int> mySet;
    mySet.insert(7);
    mySet.insert({4,9,1,5});
    set<int> newSet{move(mySet)};
    cout<<mySet.size()<<" "<<newSet.size()<<endl;

    cout<<endl<<"Conclusions on std::move "<<endl;
    cout<<"     1. It indicates that an object may be <<moved from>>."<<endl;
    cout<<"        With basic data types, it leaves the move-from object in the initial state, and the move-to object will get a copy of its value."<<endl;
    cout<<"        With containers, it leaves the move-from object in a well defined state, setting its capacity to 0 and pointer to NULL, as"<<endl;
    cout<<"        all its content is moved from it to the new object (the move to object). This happens by calling move c-tor or move operator"<<endl;
    cout<<"     2. It is exactly equivalent to a static_cast to an rvalue reference type and can be used to retrieve ravlues from lvalues"<<endl<<endl;

    //-----------------------------------------------------------------------------------------------------------------------------

    //error: direct-list-initialization of ‘auto’ requires exactly one element
    //auto obj{1,2,3};
    auto obj{"auto text"};
    cout<<typeid(obj).name()<<endl;
    string str{"new"};
    auto obj2{str};
    cout<<typeid(obj2).name()<<endl;
    auto obj3{HelperClass()};
    cout<<typeid(obj3).name()<<endl;

    HelperClass inst{3.14151, 'c'};
    const HelperClass cInst{2.7182, 'd'};

    //auto& rval {inst.getD()}; => error: cannot bind non-const lvalue reference of type ‘double&’ to an rvalue of type ‘double’
    auto& copyInst{inst};
    auto& copyCInst{cInst};
    cout<<typeid(copyInst).name()<<" "<<typeid(copyCInst).name()<<endl;
    copyInst.setC('p');
    //copyCInst.setC('p'); =>cannot modify reference to const object. Though it is specified as auto&, the constness is preserved

    //get r-value and non-const l-value in const auto&
    const auto& cCopyInst{inst};
    const auto& crval {inst.getD()};
    cout<<typeid(cCopyInst).name()<<" "<<typeid(crval).name()<<endl;

    auto&& rrval{inst.getC()};
    const auto&& crrInst{HelperClass{6.78, 'l'}};
    cout<<typeid(rrval).name()<<" "<<typeid(crrInst).name()<<endl;
    //crrInst.setC('k'); =>cannot modify reference to const object. Though it is specified as auto&, the constness is preserved

    cout<<endl<<"Conclusions on auto"<<endl;
    cout<<"     1. [C++11] For variables, specifies that the type of the variable that is being declared will be automatically deduced from its initializer."<<endl;
    cout<<"     2. auto = gets a copy of the assigned value"<<endl;
    cout<<"     2. auto& = non-const l-value reference. If the value is const, then the deducted type will be const"<<endl;
    cout<<"     3. const auto& = gets a reference to const of the assigned value. If constness is not there, it adds it, otherwise is redundant"<<endl;
    cout<<"     4. auto&& = gets a universal reference, regardless its type. If it is const, the constness is preserved."<<endl;
    cout<<"     4. const auto&& = only binds to r-values and does not allow to modify them."<<endl;


}