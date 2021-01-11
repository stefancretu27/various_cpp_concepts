#include  "includes.hpp"
#include "HelperClass.hpp"

void ReferencesAndConst()
{
    using namespace std;

    float f{0.05f}, pi{3.14159};
    const float e{2.7182};
    float& fRef{f};
    fRef = pi; //it is like performing f = pi;
    cout<<"By assigning a new object to a reference => only updates the value of the referred object. Ref: "<<fRef<<" Aliased obj: "<<f<<endl;
    fRef += 1.1f;
    cout<<"Update the value via the ref. Ref: "<<fRef<<" Aliased obj: "<<f<<" previously <<assigned>> object"<<pi<<endl;

    cout<<endl<<"Conclusions on references: "<<endl;
    cout<<"     1. References act like aliases to ojects. They must have the same type as the aliased variable, otherwise error is thrown."<<endl;
    cout<<"     2. References must be initialized upon creation, otherwise error is thrown. Pointers can be initialized any time."<<endl;
    cout<<"     3. References cannot be NULL. Pointers can be NULL."<<endl;
    cout<<"     4. References to non-const can only alias non-const l-values."<<endl;
    cout<<"     5. References can be copy constructed from other references, only that const variables must be aliased by const refs."<<endl;
    cout<<"     6. Once set to refer to an object, they cannot be reassigned to point to other object.Thus, they act like a const pointer. \
                That said, when assigning a new object to a reference, it actually updates the value of the refered to variable, not the reference itself."<<endl<<endl;

    //double& dRef{pi}; -> cannot bind non-const lvalue reference of type ‘double&’ to an rvalue of type ‘double’, as a r-value seems to be constructed from 'pi'
    const short int& siRef{pi};
    cout<<"float value: "<<pi<<" short int reference: "<<siRef<<endl; 
    //float& fRef2{e}; -> error const qualifier discared =>const reference to be used
    const float& cfRef{e}; //alias const l-value
    //cfRef = pi; -> error: const reference so the value cannot be modified
    const char& ccRef{'q'}; //alias r-value
    const char& ccRef2{ccRef};

    cout<<endl<<"Conclusions for references to const: "<<endl;
    cout<<"     1. References to const can alias const and non-const l-values, but also r-values. "<<endl;
    cout<<"     2. References to const treat the aliased pbject as const => cannot be changed via the reference. They are like const ptr to const."<<endl;
    cout<<"     3. References to const can alias objects of other types than the reference, as from the referenced object is built a r-value of the new type."<<endl;
    cout<<"     4. Used as function parameter type, prevent pass-by-value, allow r-value arguments, but the argument is const => non-modifyable"<<endl<<endl;

}