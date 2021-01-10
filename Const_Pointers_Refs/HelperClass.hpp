#ifndef HELPERCLASS_HPP
#define HELPERCLASS_HPP

class HelperClass
{
    private:
    double d;
    char c;

    public:
    HelperClass() = default;
    explicit HelperClass(const double& _d, const char& _c):d{_d},c{_c}{};

    char getC() const { return c;};
    double getD() const { return d;};
    void setC(const char& _c){c = _c;};
    void setD(const double& _d){d = _d;};
};

#endif