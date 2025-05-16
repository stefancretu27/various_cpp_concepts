#pragma once

#include <iostream>
#include <thread>
#include <chrono>

class CommonBase
{
    public:
    CommonBase() = default;
    CommonBase(const std::string& class_name) : m_class_name{class_name}
    {};

    virtual ~CommonBase() = default;
    virtual void printName() const
    {
        std::cout<<" CommonBase "<<std::endl;
    }

    virtual void doCommonBaseWork()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    private:
    const std::string m_class_name;
};

class LeftClass : virtual public CommonBase
{
    public:
    LeftClass(int i) : m_i{i}
    {};

    virtual ~LeftClass() = default;
    virtual void printName() const override
    {
        std::cout<<" LeftClass "<<std::endl;
    }

    virtual double sum(int ii)
    {
        return m_i+ii;
    }

    void doLeftClassWork()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(101));
    }

    private:
    int m_i;
};


class MiddleClass : virtual public CommonBase
{
    public:
    MiddleClass(double d) :  m_d{d}
    {};

    virtual ~MiddleClass() = default;
    virtual void printName() const override
    {
        std::cout<<" MiddleClass "<<std::endl;
    }

    virtual double computeD(double dd)
    {
        return dd*m_d;
    }

    void doMiddleClassWork()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(331));
    }

    private:
    double m_d;
};

class RightClass : virtual public CommonBase
{
    public:
    RightClass(char c) : m_c{c}
    {};

    virtual ~RightClass() = default;
    virtual void printName() const override
    {
        std::cout<<" RightClass "<<std::endl;
    }

    virtual void doRightClassWork()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(49));
    }

    private:
    char m_c;
};

class BottomClass : public LeftClass, public MiddleClass, public RightClass
{
    public:
    BottomClass(const std::string& class_name, int i, double d, char c) : CommonBase{class_name}, LeftClass{i}, MiddleClass{d}, RightClass{c}
    {};

    virtual ~BottomClass() = default;
    virtual void printName() const override
    {
        std::cout<<" BottomClass "<<std::endl;
    }

    void doBottomClassWork()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(528));
    }
};
