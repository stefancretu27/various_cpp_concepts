#ifndef ComposeFrom_HPP
#define ComposeFrom_HPP

#include <iostream>
#include <string>

using namespace std;

class ComposeFrom
{
	private:
	string name;

	public:
	ComposeFrom() = default;
	ComposeFrom(const std::string str):name{str}
	{
		cout<<"ComposeFrom c-tor"<<endl;
	};
	ComposeFrom(const ComposeFrom& n):name{n.name}
	{
		cout<<"ComposeFrom copy c-tor"<<endl;
	};
	ComposeFrom(ComposeFrom&& n):name{move(n.name)}
	{
		cout<<"ComposeFrom move c-tor"<<endl;
	}

	ComposeFrom& operator=(const ComposeFrom& n)
	{
		if(this == &n)
			return *this;

		name = n.name;

		cout<<"ComposeFrom assignment operator"<<endl;

		return *this;
	}

	string getName() const {return name;};
};

#endif