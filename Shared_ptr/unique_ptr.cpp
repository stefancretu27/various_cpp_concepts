#include "HelperClass.hpp"
#include "unique_ptr.hpp"

#define check_nullptr(up) (up)?cout<<"unique_ptr was not set to null. Its value is: "<<*up<<endl: cout<<"unique_ptr was set to nullptr"<<endl

void upFunc(const std::unique_ptr<HelperClass>& sp)
{
    std::cout<<"ref to const uPtr function parameter"<<std::endl;
}

void upFunc(std::unique_ptr<HelperClass>& sp)
{
    std::cout<<"ref to uPtr function parameter"<<std::endl;
}

void upFunc(std::unique_ptr<HelperClass>&& sp)
{
    std::cout<<"ref to r-value uPtr function parameter"<<std::endl;
    sp = std::make_unique<HelperClass>(7.17, 'g');
}

void unique_ptr_calls()
{
    using namespace std;

    unique_ptr<int> up_int{new int{7}};
    cout<<*up_int<<endl;

    //int x{9};
    //no copy c-tor or copy assignment are allowed for unique_ptr
    // unique_ptr<int>up_int_copy{up_int};
    // up_int = &x;

    //create unique_ptr from stack allocated object causes invalid free => SIGSEGV
    //unique_ptr<int> upi_inst{&x};

    unique_ptr<int>&& upInt_RefR = std::move(up_int);
    if(up_int)
    {
        cout<<"value from initial pointer: "<<*up_int<<endl;
        cout<<"value from move to ref to r-value pointer: "<<*upInt_RefR<<endl;
    }

    HelperClass helperInst{2.7182, 'k'}, secondInst{-13.13, 'q'}; 

    auto& upRef_Ref = helperInst.getUpcRef();
    cout<<"class member unique_ptr returned by ref and stored in ref variable: "<<*upRef_Ref<<endl;

    auto&& upRRef_Ref = helperInst.getUpcRef();
    cout<<"class member unique_ptr returned by ref and stored in ref to r-value variable: "<<*upRRef_Ref<<endl;

    auto&& upRef_RefR = secondInst.getUpcRefR();
    cout<<"class member unique_ptr returned by ref to r-value and stored in ref variable: "<<*upRef_RefR<<endl;

    unique_ptr<HelperClass> funcArg{new HelperClass(-12.89, 'y')};
    upFunc(funcArg);
    //upFunc(new HelperClass(1,'a')); => not accepted to build a const unique_ptr& argument => use make_unique c++14
    upFunc(make_unique<HelperClass>(1,'a')); //=>ref to r-value overload preferred to ref to const
    upFunc(move(funcArg));
    cout<<funcArg->getC()<<" "<<funcArg->getD()<<endl;

    cout<<endl<<"Conclusions on unique_ptr"<<endl;
    cout<<"     1. unique_ptr has unique ownership on a resource, so only one unique_ptr can point to that heap memory. Thus copy assignment and copy construction are not implemented"<<endl;
    cout<<"     2. Likewise shared_ptr, creating an unique_ptr from a stack allocated object (by taking its address) leads to SIGSEGV when unique_ptr d-tor calls free()."<<endl<<endl;
    cout<<"     3. A class member unique_ptr can be initialized either in member initialization list, using new or make_unique, either later, only using move semantics."<<endl<<endl;
    cout<<"     4. Returning unique_ptr from function/method can be done either by reference, with the returned value stored in a reference variable, thus avoid copy c-tor calls,"<<endl;
    cout<<"     either using move semantics, with the returned value stored in a reference to r-value."<<endl<<endl;
    cout<<"     5. A unique_ptr can be passed as argument to a function by reference, showing the intention to use an existing unique_ptr and the possibilty to change/redirect it"<<endl;
    cout<<"     Also it can be passed by reference to const, showing the intention to use an existing unique_ptr, but not to change it."<<endl<<endl;
    cout<<"     6. Having a reference to r-value to a unique_ptr, means that an existing unique_ptr can be 'moved' to it. However, the existing pointer is not left in a stable/null"<<endl;
    cout<<"     state by the move call, but rather the move call only returnes r-value ref to it. Thus, the ref to r-value still points to the same object as unique_ptr."<<endl;
    cout<<"     The only notable difference between passing unique_ptr by reference or by reference to r-value, is that in the latter case an unique_ptr can be created in place."<<endl<<endl;
    cout<<"     7. A unique_ptr can be passed by value as argument to a function. This implies moving the unique_ptr as copy c-tor cannot be called. As a result, the resource"<<endl;
    cout<<"     pointed to by the unique_ptr is moved to the newly created unique_ptr on the function's stack. This means sink, as the callee owns the object and is responisble for it"<<endl;
    cout<<"     8.Likewise having a reference to unqiue_ptr, it does not entail creating a new object, so it happens when having reference to r-value unqiue_ptr. It just creates a"<<endl;
    cout<<"     const pointer (reference implementation) to the unique_ptr, which means that the pointer can be redirected to point to another object, via such references."<<endl<<endl;
}