#pragma once

#include <iostream>
#include <string>

class TopRoot
{
    public:
    virtual ~TopRoot() = default;
    virtual void printName() {std::cout<<"Top"<<std::endl;};
    //virtual void printInt() = 0;
    virtual void doNothing(){};

};

class Top: public TopRoot
{
    public:
    Top() = default;
    virtual ~Top() = default;
    virtual void printName(){std::cout<<"Top"<<std::endl;};
    virtual void printInt(){std::cout<<i<<std::endl;};

    protected:
    int i{8};
};


class Left: virtual public Top
{
    public:
    Left() = default;
    virtual void printName(){std::cout<<"Left"<<std::endl;};
    virtual void printEuler(){std::cout<<euler<<std::endl;};

    protected:
    double euler{2.7182};
};

class Right: virtual public Top
{
    public:
    Right() = default;
    virtual void printName(){std::cout<<"Right"<<std::endl;};
    virtual void printChar(){std::cout<<c<<std::endl;};
    void printString(){std::cout<<std::string(3, c)<<std::endl;};

    protected:
    char c{'c'};
};

class Bottom: public Left, public Right
{
    public:
    Bottom() = default;
    virtual void printName() override {std::cout<<"Bottom"<<std::endl;};
    //override method from 2nd Base class
    void printEuler() override {std::cout<<"Negative Euler: "<<euler*(-1)<<std::endl;};
    //does not override as it is virtual and the inherited one is non-virtual
    virtual void printString() {std::cout<<name<<std::endl;}; 

    protected:
    std::string name{"name"};
};

class MoreBottom: public Bottom
{
    public:
    MoreBottom() = default;
    virtual void printName() override {std::cout<<"MoreBottom"<<std::endl;};
    //override method from 2nd Base class
    void printEuler() override {std::cout<<"Negative Euler: "<<euler*(2)<<std::endl;};
    //does not override as it is virtual and the inherited one is non-virtual
    void printString() override {std::cout<<"string"<<std::endl;}; 


};