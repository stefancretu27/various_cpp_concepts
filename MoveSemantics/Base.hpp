#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
#include <string>

using namespace std;

class Base
{
	protected:
	string label;

	public:
	Base() = default;
	Base(const std::string str):label{str}
	{
		cout<<"Base c-tor"<<endl;
	};
	Base(const Base& n):label{n.label}
	{
		cout<<"Base copy c-tor"<<endl;
	};
	Base(Base&& n):label{move(n.label)}
	{
		cout<<"Base move c-tor"<<endl;
	}

	Base& operator=(const Base& n)
	{
		if(this == &n)
			return *this;

		label = n.label;

		cout<<"Base assignment operator"<<endl;

		return *this;
	}

	string getLabel() const {return label;};
};

#endif