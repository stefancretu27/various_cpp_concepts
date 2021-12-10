#pragma once

#include "Root.hpp"
#include <string>
#include <utility>

class Base:public Root
{
	public:
	Base();
	Base(const std::string str);
	~Base();
	
	Base(const Base& n);
	Base& operator=(const Base& n);

	Base(Base&& n);
	Base& operator=(Base&& n);

	std::string getLabel() const;
	int getID() const;
	std::pair<int, std::string> getAttributes() const;

	//methods inherited from abstract Root class
	void pureVirtualMethod();
	//Placing override throws compilation error as it does not override, but shadows. If no override qualifier is provided, no error is thrown.
	// void virtualMethod(int i ) override
	// {
	// 	cout<<"virtual method overriden in Base class"<<endl;
	// }
	void virtualMethod() override;
	void method();

	protected:
	std::string label;

	private:
	int id{8};
};
