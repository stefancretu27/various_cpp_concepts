#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
#include <string>
#include <utility>
using namespace std;

class Root
{
	public:
	Root(){cout<<" Root c-tor"<<endl;};
	Root(const unsigned short& us){cout<<" Root c-tor with parameter"<<endl;};
	Root(const Root& r){cout<<"Root copy c-tor"<<endl;};
	Root& operator=(const Root& r){cout<<"Root copy assignment"<<endl; return *this;};
};

class Base:public Root
{
	protected:
	string label;
	static const string name;
	static  string another_name;

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
	static string getName() {return name;};
	static string getAnotherName() {return another_name;};
	static void setAnotherName(const std::string& _name) {another_name = _name;};
};

#endif