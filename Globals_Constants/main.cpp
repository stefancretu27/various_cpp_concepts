#include <iostream>

#include "globals_constants.hpp"

using namespace std;
extern void func(const int&);

int main()
{
    cout<<"const global has internal linkage: declared and initialized in .hpp each .cpp getting a separate copy of it "<<globals_constants::d<<endl;
    
    cout<<"extern const global has external linkage => forward declared in .hpp with extern and initialize din .cpp so same variable can be used";
    cout<<globals_constants::name<<endl;
    
    cout<<"extern const global has external linkage => forward declared in .hpp with extern and initialize din .cpp "<<globals_constants::c<<endl;
    
    cout<<"extern global has external linkage => forward declared in .hpp and initialized in .cpp so same variable can be used "<<globals_constants::f<<endl;
    
    cout<<"c-style symbolic constant forward declared with extern in .hpp and initialized in .cpp "<<globals_constants::symb<<endl;

    func(globals_constants::i);
    return 0;
}
