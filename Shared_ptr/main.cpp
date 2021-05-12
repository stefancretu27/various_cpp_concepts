#include "shared_ptr.hpp"
#include <iostream>

int main()
{
    using namespace std;

    cout<<endl<<"-----------------------------------------------------------------------------------------Insights on shared_ptr"<<endl;
    shared_ptr_calls();

    cout<<endl<<"-----------------------------------------------------------------------------------------Insights on weak_ptr use in cycle/lists"<<endl;
    create_shared_ptr_list();
    create_weak_ptr_list();

    return 0;
}