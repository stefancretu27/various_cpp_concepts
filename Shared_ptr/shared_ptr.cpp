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

void shared_ptr_calls()
{
    using namespace std;

    HelperClass helperInst{2.7182, 'k'};

    //create shared_ptr from existent instance using make shared, respectively new. 
    //In both cases the copy c-tor of the pointed to object is called when passed to make_shared
    shared_ptr<HelperClass> spMakeShared_Inst{make_shared<HelperClass>(helperInst)};
    shared_ptr<HelperClass> spNew_Inst{new HelperClass{helperInst}};
    cout<<spMakeShared_Inst->getD()<<" "<<spNew_Inst->getD()<<endl;

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

    cout<<endl<<"Conclusions on shared_ptr"<<endl;
    cout<<"     1. Implements shared ownership on a resource: there can be multiple shared_ptr instances pointing to the same (dynamically allocated) object"<<endl;
    cout<<"     2. Upon creation, there are dynamically allocated 2 objects: the manager object and the managed object"<<endl;
    cout<<"     The managed object is the object being pointed to whereas the manager object is created by the shared_ptr c-tor and holds"<<endl;
    cout<<"     shared counter (# of existing shared_ptrs to the manager object), weak counter (# of weak_ptr to the manager object) and a raw pointer to the managed object."<<endl;
    cout<<"     3. When creating a shared_ptr, in order to alleviate the double memory allocation, make_shared<> template method is used. It merges the two allocations into one."<<endl;
    cout<<"     4. Also, a shared_ptr can be created by passing a explicit call to new to it's c-tor. This approaches lead to 2 separate memory allocations."<<endl;
    cout<<"     5. Regardless new or make_shared are used, the argument passed to these methods can be an existing object (on stack). This would call the object's copy c-tor."<<endl;
    cout<<"     Also, regardless new or make_shared are used, the argument passed to these methods can be an anonymous object. In this case, only its c-to would be called."<<endl;
    cout<<"     6. A shared_ptr can be created from a raw pointer, with the latter passed as arg to the former's c-tor. This approache is ot be avoided, as it can lead to invalid"<<endl;
    cout<<"     memory allocation or double free attempt, as the raw pointer is directly managed by the programmer, whereas shared_ptr is managed by the class RAII concept."<<endl<<endl;

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