#include  "includes.hpp"
#include "HelperClass.hpp"

void PointersAndConst()
{
    using namespace std;

    cout<<"Relationships between pointers and const are explained here"<<endl;

    //Declare a int variable and a const int variable. Try to store them in a pointer
    int i1{};
    int *pi1{&i1};
    cout<<"int variable: "<<i1<<" dereferenced pointer to int variable: "<<*pi1<<" double preincrement value via pointer: "<<++++(*pi1)<<endl;
    const int i2{1};
    //int* pi2{&i2}; -> a const int cannot be pointed to by a pointer to non-const => needs pointer to const
    const int* pci2{&i2};
    cout<<"const int variable: "<<i2<<" dereference pointer to const int pointing to const int: "<<*pci2<<endl;
    pci2 = &i1;
    cout<<"int variable: "<<i1<<" dereferenced pointer to const int poiniting to non-const int: "<<*pci2<<endl;

    //int *pi11{pci2}; -> pointer to non-const cannot be copy constructed from pointer to const
    const int *cpi11{pi1};
    cout<<"value obtained from pointer to non-const via dereferencing cptr to const: "<<*cpi11<<endl;

    //shared_ptr to const Obj pointing to non-const obj
    shared_ptr<const HelperClass> sptrC = make_shared<HelperClass>();
    cout<<"shared_ptr to const object: "<<sptrC->getD()<<" "<<sptrC->getC()<<endl;
    //redirect shared_ptr to const to point to another object
    HelperClass inst{3.14159, 'q'};
    sptrC = make_shared<HelperClass>(inst);
    cout<<"redirected shared_ptr to const object: "<<sptrC->getD()<<" "<<sptrC->getC()<<endl;

    cout<<endl<<"Conclusions for raw/smart pointer to const: "<<endl;
    cout<<"     1. Raw/Smart pointer to non-const data type can only point to non-const variables."<<endl;
    cout<<"     2. Raw/Smart pointer to const data type can point both to const and non-const variables and treats them as being const."<<endl;
    cout<<"     3. Raw/smart pointer to const can be changed to point to another value as the pointer is non-const, only the value it points to is const"<<endl;
    cout<<"     4. Raw/smart pointer to const can be copy constructed from pointer to non-const, the pointed to object would be treated as const. \
                Pointer to non-const cannot be copy constructed from pointer to const, as const qualifier of the pointed to object would be discared"<<endl<<endl;

//---------------------------------------------------------------------------------------------------------------------------------------
    int* const cpi{&i1};
    cout<<"int variable: "<<i1<<" dereferenced const pointer to int: "<<*cpi<<" increment value via const pointer: "<<++(*cpi)<<endl;
    //int* const cpi2{&i2}; -> a const int cannot be pointed to by a const pointer to non-const => needs const pointer to const
    int* pi{cpi};
    *pi = 17;
    cout<<"int variable: "<<i1<<" dereferenced pointer to int copy constructed from const pointer used o modify the value: "<<*pi<<endl;
    int* const cpi2{pi1};
    cout<<"value obtained from const pointer to non-const via dereferencing const ptr: "<<*cpi2<<endl;

    const shared_ptr<HelperClass> cSptr = make_shared<HelperClass>(2.7182, 'w');
    cout<<"const shared_ptr to non-const object: "<<cSptr->getD()<<" "<<cSptr->getC()<<endl;
    cSptr->setC('y');
    cSptr->setD(-123.456);
    cout<<"const shared_ptr to non-const object modify values via the const shared_ptr: "<<cSptr->getD()<<" "<<cSptr->getC()<<endl;
    //cSptr = make_shared<HelperClass>(); ->const shared_ptr cannot be redirected to point to another object
    shared_ptr<HelperClass> sptr(cSptr);
    sptr->setC('p');
    cout<<"const shared_ptr to non-const object modify value via a non-const shared_ptr copy constructed from const shared_ptr: "<<cSptr->getD()<<" "<<cSptr->getC()<<endl;
    cout<<"shared count = "<<cSptr.use_count()<<" having one shared_ptr and a const shared_ptr to the same object"<<endl;

    cout<<endl<<"Conclusions for const raw/smart pointer: "<<endl;
    cout<<"     1. Can point only to non-const data types."<<endl;
    cout<<"     2. Once set to point to a variable, it cannot be reasigned to point to another variable."<<endl;
    cout<<"     3. The value it points to can be changed via pointer dereferencing as the raw/smart pointer is const the value is non-const."<<endl;
    cout<<"     4. A non-const raw/smart pointer can be copy constructed from a const raw/smart pointer. The new non-const raw/smart pointer can be\
                used for modifying"<<endl;
    cout<<"     5. A const raw/smart pointer can be copy constructed from a non-const raw/smart pointer. The const raw/smart ptr has properties 1-3"<<endl<<endl;

//-------------------------------------------------------------------------------------------------------------------------------------
    const int* const cpci1{&i1};
    cout<<"int variable: "<<i1<<" dereference const pointer to const: "<<*cpci1<<endl;
    const int* const cpci2{&i2};
    cout<<"const int variable: "<<i2<<" dereference const pointer to const: "<<*cpci2<<endl;
    const int* const cpci3{pi1};
    cout<<"value obtained from pointer to non-const via dereferencing const ptr to const: "<<*cpci3<<endl;
    const int* const cpci4{pci2};
    cout<<"value obtained from pointer to const via dereferencing const ptr to const: "<<*cpci4<<endl;
    const int* const cpci5{cpi};
    cout<<"value obtained from const pointer to non-const via dereferencing const ptr to const: "<<*cpci5<<endl;

    const shared_ptr<const HelperClass> cSptrC = make_shared<HelperClass>();
    
    cout<<endl<<"Conclusions for const pointer to const: "<<endl;
    cout<<"     1. Can point to non-const and const data types and treats them as const =>values cannot be changed via dereferencing."<<endl;
    cout<<"     2. Once set to point to a variable, it cannot be reasigned to point to another variable."<<endl;
    cout<<"     3. Can be copy constructed from any kind of pointer: const pointer to non-const and from non-const pointer to either const or non-const"<<endl;
    cout<<"     Thus, the constness is applied both to the pointer and also to the value it points to."<<endl<<endl;

}