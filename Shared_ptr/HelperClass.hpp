#ifndef HELPERCLASS_HPP
#define HELPERCLASS_HPP
#include <iostream>

class HelperClass
{
    private:
    double d;
    char c;

    public:
    HelperClass() = default;
    explicit HelperClass(const double& _d, const char& _c):d{_d},c{_c}
    {
        std::cout<<"c-tor"<<std::endl;
    };

    HelperClass(const HelperClass& inst):d{inst.d}, c{inst.c}
    {
        std::cout<<"copy c-tor"<<std::endl;
    }

    char getC() const { return c;};
    double getD() const { return d;};
    void setC(const char& _c){c = _c;};
    void setD(const double& _d){d = _d;};
};

#endif