#include "HelperClass.hpp"
#include "shared_ptr.hpp"

void func(const std::weak_ptr<HelperClass>& wp)
{
    std::cout<<"obtain shared_ptr from weak_ptr using lock() creates a new shared_ptr =>use_count is increased. ";
    if(auto&& spWp = wp.lock())
    {
        std::cout<<spWp->getD()<<" "<<spWp->getC()<<" use_count: "<<spWp.use_count()<<std::endl;
    }
    else
    {
        std::cout<<"couldn't lock weak_ptr"<<std::endl;
    }
    
}

void spFunc(const std::shared_ptr<HelperClass>& sp)
{
    std::cout<<"use count inside function taking ref to const shared_ptr argument: "<<sp.use_count()<<std::endl;
}

void spFunc(std::shared_ptr<HelperClass>& sp)
{
    std::cout<<"use count inside function taking ref to shared_ptr argument: "<<sp.use_count()<<std::endl;
}

void shared_ptr_calls()
{
    using namespace std;

    HelperClass helperInst{2.7182, 'k'}, secondInst{-13.13, 'q'};

    //create shared_ptr from existent instance using make shared, respectively new. 
    //In both cases the copy c-tor of the pointed to object is called when passed to make_shared as a copy of the object allocated on stack is created on heap.
    shared_ptr<HelperClass> spMakeShared_Inst{make_shared<HelperClass>(helperInst)};
    shared_ptr<HelperClass> spNew_Inst{new HelperClass{helperInst}};
    cout<<spMakeShared_Inst->getD()<<" "<<spMakeShared_Inst->getC()<<" "<<spNew_Inst->getD()<<" "<<spNew_Inst->getC()<<endl;

    spMakeShared_Inst->setC('x');
    spMakeShared_Inst->setD(0.123);
    cout<<"values changed via one of the shared_ptrs created by copy constructing on heap from the same stack allocated object not seen on the other shared_ptr "<<spNew_Inst->getC()<<" "<<spNew_Inst->getD()<<endl;

    //change values of pointed to instance
    helperInst.setC('F');
    helperInst.setD(73.658);
    cout<<"The changes not visible via shared_ptr dereference as they point to copies of the instance, which were made on heap: "<<spMakeShared_Inst->getD()<<" "<<spNew_Inst->getD()<<endl;

    cout<<"take a shared_ptr to stack allocated object via its address => d-tor called twice causing double free memory corruption, even if shared_ptr is nulled/redirected"<<endl;
    // shared_ptr<HelperClass> spAddr_Inst{&secondInst};
    // cout<<spAddr_Inst->getC()<<" "<<spAddr_Inst->getD()<<endl;
    // spAddr_Inst = make_shared<HelperClass>(44.4, 'y');


    //create shared_ptr using make shared, respectively new, for anonymous object.  Copy c-tor is ellided for anonymous objects. 
    //Instead the object is constructed in place, so the c-tor is called when make_shared is invoked.
    shared_ptr<HelperClass> spMakeShared_Obj{make_shared<HelperClass>(3.14159, 'l')};
    shared_ptr<HelperClass> spNew_Obj{new HelperClass(-273.7, 'q')};
    cout<<spMakeShared_Obj->getD()<<" "<<spNew_Obj->getD()<<endl;

    //create shared_ptr from another shared_ptr=>shared_ptr copy c-tor is called
    shared_ptr<HelperClass> spCopy{spMakeShared_Inst};
    cout<<spCopy->getC()<<endl;

    //create shared_ptr from raw pointer => shared_ptr c-tor is called
    HelperClass* hpPtr{new HelperClass{-12,'b'}};
    shared_ptr<HelperClass> spPtr{hpPtr};
    cout<<spPtr->getC()<<endl;

    //get shared_ptr member form class, both as ref and as copy
    //cout<<"reference to shared_ptr member variable use count: "<<spMakeShared_Obj->getSpdRef().use_count()<<endl;
    auto& spdRef_Ref = spMakeShared_Obj->getSpdRef();
    cout<<"retrieve as reference to shared_ptr member variable stored in reference: "<<*spdRef_Ref<<" use count: "<<spdRef_Ref.use_count()<<endl;

    auto spdCopy_Ref = spMakeShared_Obj->getSpdRef();
    cout<<"retrieve as reference to shared_ptr member variable stored in variable => shared_ptr copy c-tor called when creating the variable: "<<*spdCopy_Ref<<" use count: "<<spdCopy_Ref.use_count()<<endl;

    auto&& spdRRef_Copy = spMakeShared_Obj->getSpdCopy();
    cout<<"retrieve as copy to shared_ptr member variable stored in reference => shared_ptr copy c-tor called by the getter: "<<*spdRRef_Copy<<" use count: "<<spdRRef_Copy.use_count()<<endl;

    auto spdCopy_Copy = spMakeShared_Obj->getSpdCopy();
    cout<<"retrieve as copy to shared_ptr member variable stored in variable => shared_ptr copy c-tor called once due to copy ellision "<<*spdCopy_Copy<<" use count: "<<spdCopy_Copy.use_count()<<endl;
    *spdCopy_Copy = -73.4;
    cout<<"class member value set via copy to shared_ptr "<<*spMakeShared_Obj->getSpdRef()<<" value pointed to by member shared_ptr: "<<spMakeShared_Obj->getDRef()<<endl;
    *spdRef_Ref = -19.09;
    cout<<"class member value set via ref to shared_ptr "<<*spMakeShared_Obj->getSpdRef()<<" value pointed to by member shared_ptr: "<<spMakeShared_Obj->getDRef()<<endl;
    double& refToMember = spMakeShared_Obj->getDRef();
    refToMember = 1295.38;
    cout<<"class member value set via ref to member "<<spMakeShared_Obj->getDRef()<<endl;

    spFunc(spNew_Obj);
    const shared_ptr<HelperClass>& spRefConst{spNew_Obj};
    std::cout<<"use count after creating ref to const shared_ptr from existing shared_ptr: "<<spRefConst.use_count()<<std::endl;
    spFunc(make_shared<HelperClass>(19.19, 'v'));

    cout<<endl<<"Conclusions on shared_ptr"<<endl;
    cout<<"     1. Implements shared ownership on a resource: there can be multiple shared_ptr instances pointing to the same (dynamically allocated) object"<<endl;
    cout<<"     If two/more shared_ptrs are created to point to the same object via make_shared/new, but not via copy constructing, each will allocate a different object on heap"<<endl;
    cout<<"     and each will have a distinct manager object that is allocated each time. Thus two/more sahred_ptrs can point to the same memory location via copy construction."<<endl;
    cout<<"     2. Upon creation, there are dynamically allocated 2 objects: the manager object and the managed object"<<endl;
    cout<<"     The managed object is the object being pointed to whereas the manager object is created by the shared_ptr c-tor and holds"<<endl;
    cout<<"     shared counter (# of existing shared_ptrs to the manager object), weak counter (# of weak_ptr to the manager object) and a raw pointer to the managed object."<<endl;
    cout<<"     3. When creating a shared_ptr, in order to alleviate the double memory allocation, make_shared<> template method is used. It merges the two allocations into one."<<endl;
    cout<<"     4. Also, a shared_ptr can be created by passing a explicit call to new to it's c-tor. This approaches lead to 2 separate memory allocations."<<endl;
    cout<<"     5. Regardless new or make_shared are used, the argument passed to these methods can be an existing object (on stack). This would call the object's copy c-tor."<<endl;
    cout<<"     as a copy of the stack allocated object is created on heap. That said, the newly created shared_ptr will not point to the stack allocated object, but to its copy on heap."<<endl;
    cout<<"     6. Also, regardless new or make_shared are used, the argument passed to these methods can be an anonymous object. In this case, only its c-tor would be called."<<endl;
    cout<<"     That is because the pointed to object is constructed on heap."<<endl;
    cout<<"     7. A shared_ptr can be created from a raw pointer, with the latter passed as arg to the former's c-tor. This approach is to be avoided, as it can lead to invalid"<<endl;
    cout<<"     memory allocation or double free attempt, as the raw pointer is directly managed by the programmer, whereas shared_ptr is managed by the class RAII concept."<<endl;
    cout<<"     Also, a shared_ptr can be created from the address of a stack allocated object, but upon destruction the object's d-tor is called twice resulting in double free"<<endl;
    cout<<"     7. A shared_ptr member variable to a class can be gotten via copy or reference, as an appropriate getter can be implemented for each of these cases"<<endl;
    cout<<"     If the returned ref to shared_ptr is not stored in a reference variable (either & or &&), but in a normal variable, a copy of the shared_ptr will be created anyway."<<endl;
    cout<<"     8. A shared_ptr that is local variable of a function can only be returned via copy. Otherwise, pointer/ref to local variable would be returned, causing error,"<<endl;
    cout<<"     as the shared_ptr itself is a stack allocated object, as it is any pointer. It just points to heap allocated memory."<<endl;
    cout<<"     9. Passing a shared_ptr by value as function argument denotes intention to share ownership to the heap data with other existing shared_ptr (at least with the one,"<<endl;
    cout<<"     that it is copied from). Also the pointer can be changed/redirected inside the function. Passing by const value denotes that the pointer will not be changed/redirected"<<endl;
    cout<<"     10. Passing a shared_ptr by ref as function argument denotes possibility of modifying the pointer inside the function. The exsiting ownership is reused and no extra"<<endl;
    cout<<"     memory allocations are done as no copy is made. Passing the shared_ptr by ref to const denotes there is no intention to modify the shared_ptr inside the function."<<endl<<endl;

    //------------------------------------------------------------------------------------------------------------------------
    weak_ptr<HelperClass> wp{spNew_Inst};
    //cout<<wp->getC()<<endl;
    weak_ptr<HelperClass> wpCopy{wp};
    cout<<"weak_ptr use_count() actually returns the number of shared_ptr that point to the object: "<<wpCopy.use_count()<<endl;

    //spNew_Inst.reset();
    shared_ptr<HelperClass> spWp{wp};
    cout<<"Obtain shared_ptr from weak_ptr via copy c-tor. If the initial shared_ptr had been deleted, this causes seg fault. "<<spWp.use_count()<<endl;

    spNew_Inst.reset();
    cout<<"After reset initial shared_ptr, use count is: "<<spWp.use_count()<<endl;
    spWp.reset();
    cout<<"After reset shared_ptr created from weak_ptr via copy c-tor, use count is: "<<spWp.use_count()<<endl;
    auto spWp2{wp.lock()};
    cout<<"After creation of shared_ptr from weak_ptr, use count is: "<<spWp2.use_count()<<" because there is no managed object. Thus, the returned shared_ptr is empty."<<endl;

    //pass shared_ptr as arg to function taking weak_ptr parameter => the weak_ptr is constructed from the shared_ptr arg
    cout<<"Use_count before calling func with weak_ptr parameter: "<<spMakeShared_Obj.use_count()<<endl;
    func(spMakeShared_Obj);

    //get a weak_ptr to ashared_ptr returned from method 
    weak_ptr<double> wpd = spNew_Obj->getSpdRef();
    cout<<"use count of shared_ptr member retrieved via weak_ptr constructed from getter: "<<wpd.use_count()<<endl;

    cout<<endl<<"Conclusions on weak_ptr"<<endl;
    cout<<"     1. It is used in conjunction with shared_ptr and acts like an observer to the manager object."<<endl;
    cout<<"     2. It cannot be used for dereferencing. There can be more weak_ptrs to the same manager object."<<endl;
    cout<<"     3. Weak_ptr can be created via copy constructing or copy assignment from existing shared_ptr or weak_ptr. The weak count is increased accordingly. "<<endl;
    cout<<"     4. Also, a shared_ptr can be created from a weak_ptr. The preferred approach is to use the lock() method on an existing weak_ptr, this returning a new shared_ptr."<<endl;
    cout<<"     The returned pointer should be checked against null. This also can be used to pass shared_ptr as argument to function whose parameter is a weak_ptr."<<endl;
    cout<<"     Here, firstly the weak_ptr is created on the function's stack from the passed shared_ptr. Inside the function, lock() is used to get a new shared_ptr."<<endl;
    cout<<"     If there is another shared_ptr in existence, a new one is created, often this representing a downside from memory allocation perspective."<<endl;
    cout<<"     5. If the shared_ptr is created via copy c-tor from a weak_ptr, if the inital shared_ptr had been deallocated, the creation would fail with SIGSEGV."<<endl<<endl;
}