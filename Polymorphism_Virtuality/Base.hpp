#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
#include <string>
#include <utility>
using namespace std;

class Root
{
	public:
	virtual void pureVirtualMethod() = 0;
	virtual void virtualMethod()
	{
		cout<<"virtual method implemented inside abstract class"<<endl;
	}
	void method()
	{
		cout<<"method implemented inside abstract class"<<endl;
	}

};

void Root::pureVirtualMethod()
{
	cout<<"pure virtual method implemented outside abstract class"<<endl;
};

class Base:public Root
{
	protected:
	string label;

	private:
	int id{8};

	public:
	Base(){cout<<" Base default c-tor"<<endl;};
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

		cout<<"Base copy assignment operator"<<endl;

		return *this;
	}

	Base& operator=(Base&& n)
	{
		if(this == &n)
			return *this;

		label = move(n.label);

		cout<<"Base move assignment operator"<<endl;

		return *this;
	}

	string getLabel() const {return label;};
	int getID() const {return id;};
	pair<int, string> getAttributes() const { return make_pair(id, label);};

	//methods inherited from abstract Root class
	void pureVirtualMethod()
	{
		cout<<"pure virtual method overriden in Base class"<<endl;
	}
	//Placing override throws compilation error. If no override qualifier is provided, no error is thrown.
	// void virtualMethod(int i ) override
	// {
	// 	cout<<"virtual method overriden in Base class"<<endl;
	// }
	void virtualMethod() override
	{
		cout<<"virtual method overriden in Base class"<<endl;
	}
	void method()
	{
		cout<<"method overriden in Base class"<<endl;
	}
};

#endif