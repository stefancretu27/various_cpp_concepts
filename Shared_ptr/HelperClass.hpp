#ifndef HELPERCLASS_HPP
#define HELPERCLASS_HPP
#include <iostream>
#include <memory>

class HelperClass
{
    private:
    double d;
    char c;
    std::shared_ptr<double> mSpd;

    public:
    HelperClass() = default;
    explicit HelperClass(const double& _d, const char& _c):d{_d},c{_c}
    {
        mSpd = std::make_shared<double>(d);
        std::cout<<"c-tor"<<std::endl;
    };

    HelperClass(const HelperClass& inst):d{inst.d}, c{inst.c}
    {
        std::cout<<"copy c-tor"<<std::endl;
    }

    char getC() const { return c;};
    double getD() const { return d;};
    double& getDRef()  { return d;};
    std::shared_ptr<double>& getSpdRef() {return mSpd;};
    std::shared_ptr<double> getSpdCopy() {return mSpd;};
    void setC(const char& _c){c = _c;};
    void setD(const double& _d){d = _d;};
};

#endif