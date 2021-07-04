#ifndef DERIVEDIMPL_HPP
#define DERIVEDIMPL_HPP

#include <tuple>
#include "Base.hpp"
using namespace std;

class DerivedImpl:public Base
{
	protected:
	double data;

	public:
	DerivedImpl() = default;

	DerivedImpl(const std::string& str, const double& _data):Base{str}, data{_data}
	{
		cout<<"DerivedImpl c-tor that explicitly calls Base c-tor"<<endl;
	};

	DerivedImpl(const std::string& str, const double& _data, const bool& _x):data{_data}
	{
		cout<<"DerivedImpl c-tor that does not call Base c-tor"<<endl;
	};

	//overoad copy c-tor
	DerivedImpl(const DerivedImpl& n) = default;

	DerivedImpl(const DerivedImpl& n, const bool& x):Base{n}, data{n.data}
	{
		cout<<"DerivedImpl copy c-tor that explicitly calls Base copy c-tor"<<endl;
	};

	DerivedImpl(const DerivedImpl& n, const int& x): data{n.data}
	{
		cout<<"DerivedImpl copy c-tor that does not call Base copy c-tor"<<endl;
	};

	DerivedImpl& operator=(const DerivedImpl& n)
	{
		if(this == &n)
			return *this;

		data = n.data;
		//explicitly call Base class copy assignment
        Base::operator=(n);
		//or perform memberiwse assignment between attributes inherited from Base + deep copy is the case
		//label = n.label;

		cout<<"DerivedImpl copy assignment operator"<<endl;

		return *this;
	}

	//move semantics
	DerivedImpl(DerivedImpl&& n):Base{move(n)}, data{n.data}
	{
		//leave arg object in well defined state
        n.data = 0.0f;
		cout<<"DerivedImpl move c-tor"<<endl;
	}

	DerivedImpl& operator=(DerivedImpl&& n)
	{
		if(this == &n)
			return *this;

		data = n.data;
        //leave arg object in well defined state
        n.data = 0.0f;
        Base::operator=(move(n));

		cout<<"DerivedImpl move assignment operator"<<endl;

		return *this;
	}

	double getData() const {return data;};
	tuple<int, string, double> getAttributes() const { return make_tuple(this->getID(), label, data);};

	static void setAnotherName(const std::string& _name) 
	{
		cout<<"Override static method from Base class in Derived class"<<endl;
		another_name = _name;
	};
};

#endif