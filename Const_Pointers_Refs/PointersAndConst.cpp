#include  "includes.hpp"
#include "HelperClass.hpp"

void PointersAndConst()
{
    using namespace std;

    cout<<"Relationships between pointers and const are explained here"<<endl;

    //Declare a int variable and construct a pointer to non-const from it
    int intVal{};
    int *ptr_IntVal{&intVal};
    cout<<"int variable: "<<intVal<<" dereferenced pointer to int variable: "<<*ptr_IntVal<<" double preincrement value via pointer: "<<++++(*ptr_IntVal)<<endl;
    
    //Declare a const int variable and construct a pointer to const from it. Then change pointer to const to point to the above-declared int
    const int constIntVal{1};
    //int* pconstIntVal{&constIntVal}; -> a const int cannot be pointed to by a pointer to non-const => needs pointer to const
    const int* ptr_ConstIntVal{&constIntVal};
    cout<<"const int variable: "<<constIntVal<<" dereferenced pointer to const int constructed from const int: "<<*ptr_ConstIntVal<<endl;
    ptr_ConstIntVal = &intVal;
    cout<<"int variable: "<<intVal<<" dereferenced pointer to const int constructed from non-const int: "<<*ptr_ConstIntVal<<endl;

    //pointer to non-const int cannot be copy constructed from pointer to const
    //int *ptr_IntPtr{ptr_ConstIntVal}; 
    //create pointer to const int from pointer to non-const int
    const int *ptr_ConstInt{ptr_IntVal};
    cout<<"dereferenced ptr to const int constructed from pointer to non-const int: "<<*ptr_ConstInt<<endl;

    //shared_ptr to const Obj ->non-const obj
    shared_ptr<const HelperClass> sPtr_ConstInst = make_shared<HelperClass>();
    cout<<"shared_ptr to const object: "<<sPtr_ConstInst->getD()<<" "<<sPtr_ConstInst->getC()<<endl;
    //redirect shared_ptr to const to point to another object
    HelperClass inst{3.14159, 'q'};
    sPtr_ConstInst = make_shared<HelperClass>(inst);
    cout<<"redirected shared_ptr to const object: "<<sPtr_ConstInst->getD()<<" "<<sPtr_ConstInst->getC()<<endl;

    cout<<endl<<"Conclusions for raw/smart pointer to const: "<<endl;
    cout<<"     1. Raw/Smart pointer to non-const data type can only point to non-const variables."<<endl;
    cout<<"     2. Raw/Smart pointer to const data type can point both to const and non-const variables and treats them as being const."<<endl;
    cout<<"     3. Raw/smart pointer to const can be changed to point to another value as the pointer is non-const, only the value it points to is const"<<endl;
    cout<<"     4. Raw/smart pointer to const can be copy constructed from pointer to non-const, the pointed to object would be treated as const."<<endl; 
    cout<<"        Pointer to non-const cannot be copy constructed from pointer to const, as const qualifier of the pointed to object would be discared"<<endl<<endl;

//---------------------------------------------------------------------------------------------------------------------------------------
    //declare const ptr to int -> int val
    int* const constPtr_IntVal{&intVal};
    cout<<"int variable: "<<intVal<<" dereferenced const pointer to int: "<<*constPtr_IntVal<<" increment value via const pointer: "<<++(*constPtr_IntVal)<<endl;
    //int* const constPtr_IntValPtr{&constIntVal}; -> a const int cannot be pointed to by a const pointer to non-const => needs const pointer to const
    
    //declare pointer to int -> the above-declared const pointer
    int* ptrInt{constPtr_IntVal};
    *ptrInt = 17;
    cout<<"int variable: "<<intVal<<" dereferenced pointer to int copy constructed from const pointer used to modify the value: "<<*ptrInt<<endl;
    int* const constPtrIntPtr{ptr_IntVal};
    cout<<"dereferenced const ptr constructed from ptr to int: "<<*constPtrIntPtr<<endl;

    const shared_ptr<HelperClass> constSPtr_Inst = make_shared<HelperClass>(2.7182, 'w');
    cout<<"const shared_ptr to non-const object: "<<constSPtr_Inst->getD()<<" "<<constSPtr_Inst->getC()<<endl;
    constSPtr_Inst->setC('y');
    constSPtr_Inst->setD(-123.456);
    cout<<"const shared_ptr to non-const object modify values via the const shared_ptr: "<<constSPtr_Inst->getD()<<" "<<constSPtr_Inst->getC()<<endl;
    //constSPtr_Inst = make_shared<HelperClass>(); ->const shared_ptr cannot be redirected to point to another object
    shared_ptr<HelperClass> sptr(constSPtr_Inst);
    sptr->setC('p');
    cout<<"const shared_ptr to non-const object modify value via a non-const shared_ptr copy constructed from const shared_ptr: "<<constSPtr_Inst->getD()<<" "<<constSPtr_Inst->getC()<<endl;
    cout<<"shared count = "<<constSPtr_Inst.use_count()<<" having one shared_ptr and a const shared_ptr to the same object"<<endl;

    cout<<endl<<"Conclusions for const raw/smart pointer: "<<endl;
    cout<<"     1. Can point only to non-const data types."<<endl;
    cout<<"     2. Once set to point to a variable, it cannot be reasigned to point to another variable."<<endl;
    cout<<"     3. The value it points to can be changed via pointer dereferencing as the raw/smart pointer is const the value is non-const."<<endl;
    cout<<"     4. A non-const raw/smart pointer can be copy constructed from a const raw/smart pointer. The new non-const raw/smart pointer can be"<<endl;
    cout<<"        used for modifying"<<endl;
    cout<<"     5. A const raw/smart pointer can be copy constructed from a non-const raw/smart pointer. The const raw/smart ptr has properties 1-3"<<endl<<endl;

//-------------------------------------------------------------------------------------------------------------------------------------
    const int* const constPtrToConst_IntVal{&intVal};
    cout<<"int variable: "<<intVal<<" dereferenced const ptr to const -> int: "<<*constPtrToConst_IntVal<<endl;
    const int* const constPtrToConst_ConstIntVal{&constIntVal};
    cout<<"const int variable: "<<constIntVal<<" dereferenced const ptr to const -> const int: "<<*constPtrToConst_ConstIntVal<<endl;
    const int* const constPtrToConst_PtrInt{ptr_IntVal};
    cout<<"ptr to non-const "<<*ptr_IntVal<<" dereferenced const ptr to const constructed from ptr to non-const : "<<*constPtrToConst_PtrInt<<endl;
    const int* const constPtrToConst_PtrConstInt{ptr_ConstIntVal};
    cout<<"ptr to const "<<*ptr_IntVal<<" dereferenced const ptr to const constructed from ptr to const int: "<<*constPtrToConst_PtrConstInt<<endl;
    const int* const constPtrToConst_ConstPtrInt{constPtr_IntVal};
    cout<<"const ptr to non-const "<<*constPtr_IntVal<<" dereferenced const ptr to const obtained from const ptr to non-const: "<<*constPtrToConst_ConstPtrInt<<endl;

    const shared_ptr<const HelperClass> constSPtr_ConstInst = make_shared<HelperClass>();
    cout<<"dereferencing const shared ptr to const instance: "<<constSPtr_ConstInst->getD()<<" "<<constSPtr_ConstInst->getC()<<endl;
    
    cout<<endl<<"Conclusions for const pointer to const: "<<endl;
    cout<<"     1. Can point to non-const and const data types and treats them as const =>values cannot be changed via dereferencing."<<endl;
    cout<<"     2. Once set to point to a variable, it cannot be reasigned to point to another variable."<<endl;
    cout<<"     3. Can be copy constructed from any kind of pointer: const pointer to non-const and from non-const pointer to either const or non-const"<<endl;
    cout<<"     Thus, the constness is applied both to the pointer and also to the value it points to."<<endl<<endl;

}