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
    std::unique_ptr<char> mUpc;

    public:
    HelperClass() = default;
    explicit HelperClass(const double& _d, const char& _c):d{_d},c{_c}, mUpc{new char{c}}
    {
        mSpd = std::make_shared<double>(d);
        //unique ptr must be initialized upon creation as assignment operator is not implemented. However, move semantics can be used for initialization
        //mUpc = new char;
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
    std::unique_ptr<char>& getUpcRef() {return mUpc;};
    std::unique_ptr<char>&& getUpcRefR() {return std::move(mUpc);};

    void setC(const char& _c){c = _c;};
    void setD(const double& _d){d = _d;};
};

#endif