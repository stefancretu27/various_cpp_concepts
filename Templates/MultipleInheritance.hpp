#pragma once

#include <iostream>
#include <string>

class Base1
{
    public:
    Base1() = default;
    ~Base1() = default;
    virtual void printName(){std::cout<<"Base1"<<std::endl;};
    virtual void printInt(){std::cout<<i<<std::endl;};

    protected:
    int i{8};
};


class Base2
{
    public:
    Base2() = default;
    virtual void printName(){std::cout<<"Base2"<<std::endl;};
    virtual void printEuler(){std::cout<<euler<<std::endl;};

    public:
    double euler{2.7182};
};

class Base3
{
    public:
    Base3() = default;
    virtual void printName(){std::cout<<"Base3"<<std::endl;};
    virtual void printChar(){std::cout<<c<<std::endl;};
    void printString(){std::cout<<std::string(3, c)<<std::endl;};

    protected:
    char c{'c'};
};

class Derived: public Base1, public Base2, public Base3
{
    public:
    Derived() = default;
    //override method inherited from all Base classes
    void printName() override {std::cout<<"Derived"<<std::endl;};
    //override method from 2nd Base class
    void printEuler() override {std::cout<<"Negative Euler: "<<euler*(-1)<<std::endl;};
    virtual void printString() {std::cout<<name<<std::endl;}; //does not override as it is virtual and the inherited one is non-virtual

    private:
    std::string name{"name"};
};
