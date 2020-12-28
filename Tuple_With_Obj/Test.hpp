#include <utility> 
#include <tuple>

class Test
{
private:
	double d;
	char c;
public:
	Test():d(0.0),c('a'){};
	Test(const double& dval, const char& cval):d(dval), c(cval){};
	~Test(){};
	
	double getD() const {return d;};
	char getC() const {return c;};
	void setD(const double& dval) {d = dval;};
	void setC(const char& cval) {c = cval;};
	std::tuple<double, char> getTuple() {return std::make_tuple(d, c);};
	
	friend bool operator==(const Test& in1, const Test & in2)
	{
		return (in1.d==in2.d)&&(in1.c==in2.c);
	}

};
