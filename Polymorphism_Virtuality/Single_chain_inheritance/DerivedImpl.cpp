#include "DerivedImpl.hpp"

#include <iostream>
using namespace std;

DerivedImpl::DerivedImpl()
{
    cout<<"     DerivedImpl default c-tor"<<endl;
}

DerivedImpl::~DerivedImpl()
{
    cout<<"     DerivedImpl dtor"<<endl;
}

DerivedImpl::DerivedImpl(const std::string& str, const double& _data):Base{str}, data{_data}
{
    cout<<"     DerivedImpl c-tor that explicitly calls Base c-tor"<<endl;
}

DerivedImpl::DerivedImpl(const std::string& str, const double& _data, const bool& _x):data{_data}
{
    cout<<"     DerivedImpl c-tor that does not call Base c-tor"<<endl;
}

//overload copy c-tor
DerivedImpl::DerivedImpl(const DerivedImpl& n, const bool& x):Base{n}, data{n.data}
{
    cout<<"     DerivedImpl copy c-tor that explicitly calls Base copy c-tor"<<endl;
}

DerivedImpl::DerivedImpl(const DerivedImpl& n, const int& x): data{n.data}
{
    cout<<"     DerivedImpl copy c-tor that does not call Base copy c-tor"<<endl;
}

DerivedImpl& DerivedImpl::operator=(const DerivedImpl& n)
{
    if(this == &n)
        return *this;

    data = n.data;
    //explicitly call Base class copy assignment
    Base::operator=(n);
    //or perform memberiwse assignment between attributes inherited from Base + deep copy is the case
    //label = n.label;

    cout<<"     DerivedImpl copy assignment operator"<<endl;

    return *this;
}

//move semantics
DerivedImpl::DerivedImpl(DerivedImpl&& n):Base{move(n)}, data{n.data}
{
    //leave arg object in well defined state
    n.data = 0.0f;
    cout<<"     DerivedImpl move c-tor"<<endl;
}

DerivedImpl& DerivedImpl::operator=(DerivedImpl&& n)
{
    if(this == &n)
        return *this;

    data = n.data;
    //leave arg object in well defined state
    n.data = 0.0f;
    Base::operator=(move(n));

    cout<<"     DerivedImpl move assignment operator"<<endl;

    return *this;
}

double DerivedImpl::getData() const 
{
    return data;
}

tuple<int, string, double> DerivedImpl::getAttributes() const 
{ 
    return make_tuple(this->getID(), label, data);
}

//methods inherited from abstract Root class
void DerivedImpl::pureVirtualMethod()
{
    cout<<"     pure virtual method overriden in DerivedImpl class"<<endl;
}

void DerivedImpl::virtualMethod()
{
    cout<<"     virtual method overriden in DerivedImpl class"<<endl;
}

void DerivedImpl::method()
{
    cout<<"     method overriden in DerivedImpl class"<<endl;
}