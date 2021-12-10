#include "Root.hpp"

#include <iostream>
using namespace std;

Root::Root()
{
    //virtual methods call in c-tor should be avoided, as the vptr for the object in construction is set when the c-tor is invoked
    cout<<"Virtual method call inside Root ctor"<<endl;
    virtualMethod();
    cout<<"Root ctor"<<endl;
}

Root::~Root()
{
    cout<<"Virtual method call inside Root dtor"<<endl;
    virtualMethod();
    cout<<"Root dtor"<<endl;
}

void Root::pureVirtualMethod()
{
	cout<<"pure virtual method implemented outside abstract class"<<endl;
};

void Root::virtualMethod()
{
    cout<<"virtual method implemented inside abstract class"<<endl;
}

void Root::method()
{
    cout<<"method implemented inside abstract class"<<endl;
}