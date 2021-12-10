#include "Base.hpp"

#include <iostream>
using namespace std;

Base::~Base() 
{
    cout<<" Base d-tor"<<endl;
}

Base::Base()
{
    cout<<" Base default c-tor"<<endl;
}

Base::Base(const std::string str):label{str}
{
    cout<<" Base c-tor"<<endl;
};
	
Base::Base(const Base& n):label{n.label}
{
    cout<<" Base copy c-tor"<<endl;
};

Base::Base(Base&& n):label{move(n.label)}
{
    cout<<" Base move c-tor"<<endl;
}

Base& Base::operator=(const Base& n)
{
    if(this == &n)
        return *this;

    label = n.label;

    cout<<" Base copy assignment operator"<<endl;

    return *this;
}

Base& Base::operator=(Base&& n)
{
    if(this == &n)
        return *this;

    label = move(n.label);

    cout<<" Base move assignment operator"<<endl;

    return *this;
}

string Base::getLabel() const 
{
    return label;
}

int Base::getID() const 
{
    return id;
}

pair<int, string> Base::getAttributes() const 
{ 
    return make_pair(id, label);
}


//methods inherited from abstract Root class
void Base::pureVirtualMethod()
{
    cout<<" pure virtual method overriden in Base class"<<endl;
}

//override qualifier is already placed in header file
void Base::virtualMethod()
{
    cout<<" virtual method overriden in Base class"<<endl;
}

void Base::method()
{
    cout<<" method overriden in Base class"<<endl;
}