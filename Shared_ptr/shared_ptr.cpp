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
    //In both cases the copy c-tor of the pointed to object when passed to make_shared
    shared_ptr<HelperClass> spMakeShared_Inst{make_shared<HelperClass>(helperInst)};
    shared_ptr<HelperClass> spNew_Inst{new HelperClass{helperInst}};
    cout<<spMakeShared_Inst->getD()<<" "<<spNew_Inst->getD()<<endl;

    //create shared_ptr using make shared, respectively new, for anonymous object.  Copy c-tor is ellided for anonymous objects. 
    //Instead the object is constructed in place, so the c-tor is called when make_shared is invoked.
    shared_ptr<HelperClass> spMakeShared_Obj{make_shared<HelperClass>(3.14159, 'l')};
    shared_ptr<HelperClass> spNew_Obj{make_shared<HelperClass>(-273.7, 'q')};
    cout<<spMakeShared_Obj->getD()<<" "<<spNew_Obj->getD()<<endl;

    //create shared_ptr from another shared_ptr=>shared_ptr copy c-tor is called
    shared_ptr<HelperClass> spCopy{spMakeShared_Inst};
    cout<<spCopy->getC()<<endl;

    //create shared_ptr from raw pointer => shared_ptr c-tor is called
    HelperClass* hpPtr{new HelperClass{-12,'b'}};
    shared_ptr<HelperClass> spPtr{hpPtr};
    cout<<spPtr->getC()<<endl;

    //pass shared_ptr as arg to function taking weak_ptr parameter => the weak_ptr is constructed from the shared_ptr arg
    cout<<"use_count before calling func with weak_ptr parameter: "<<spMakeShared_Obj.use_count()<<endl;
    func(spMakeShared_Obj);
    
}