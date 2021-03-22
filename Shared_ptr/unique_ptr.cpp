#include "HelperClass.hpp"
#include "unique_ptr.hpp"

#define check_nullptr(up) (up)?cout<<"unique_ptr was not set to null. Its value is: "<<*up<<endl: cout<<"unique_ptr was set to nullptr"<<endl

void upFunc(const std::unique_ptr<HelperClass>& sp)
{

}

void upFunc(std::shared_ptr<HelperClass>& sp)
{

}

void unique_ptr_calls()
{
    using namespace std;

    unique_ptr<int> up_int{new int{3}};
    cout<<*up_int<<endl;

    int x{9};
    //no copy c-tor or copy assignment are allowed for unique_ptr
    // unique_ptr<int>up_int_copy{up_int};
    // up_int = &x;

    //create unique_ptr from stack alloctaed object causes invalid free => SIGSEGV
    //unique_ptr<int> upi_inst{&x};

    cout<<endl<<"Conclusions on unique_ptr"<<endl;
    cout<<"     1. unique_ptr has unique ownership on a resource, so only one unique_ptr can point to that heap memory. Thus copy assignment and copy construction are not implemented"<<endl;
    cout<<"     2. Likewise shared_ptr, creating an unique_ptr from astack allocated object (by taking its address) leads to SIGSEGV when unique_ptr d-tor calls free()."<<endl<<endl;
}