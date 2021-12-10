#pragma once

#include <tuple>
#include "Base.hpp"


class DerivedImpl:public Base
{
	public:
	DerivedImpl();
	DerivedImpl(const std::string& str, const double& _data);
	//overload c-tor with params for the sake of syntax
	DerivedImpl(const std::string& str, const double& _data, const bool& _x);
	~DerivedImpl();

	DerivedImpl(const DerivedImpl& n) = default;
	//overload copy c-tor for the sake of syntax
	DerivedImpl(const DerivedImpl& n, const bool& x);
	DerivedImpl(const DerivedImpl& n, const int& x);

	DerivedImpl& operator=(const DerivedImpl& n);

	//move semantics
	DerivedImpl(DerivedImpl&& n);
	DerivedImpl& operator=(DerivedImpl&& n);

	double getData() const;
	std::tuple<int, std::string, double> getAttributes() const;

	//methods inherited from abstract Root class
	void pureVirtualMethod() override;
	void virtualMethod() override;
	void method();

	protected:
	double data;
};
