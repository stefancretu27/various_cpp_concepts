#include <iostream>
#include <algorithm>
#include <memory>
#include <functional>


#define bool_to_word(x) ((x) ? "true" : "false")

using namespace std;

//use std::function as param type for a function which calls a lambda, as auto cannot be used for functions' parameters
void func(const function<const char* (bool)> &fn, bool arg)
{
	cout<<"lambda called from function whose arg is std::function "<<fn(arg)<<endl;
}

//apply some math on input a
double operation(double a)
{
	double pi{3.14}, e{2.71};
	
	//capture variables defined in the same scope as the lambda variable
	//pi is captured by value, e by ref => e can be modified internally by the lambda
	auto lambda([pi, &e](double in)
				{
					e=e*1.1;
					return in*pi + e;
				});
	
	return lambda(a);
}

//declare lambda as mutable, so the variables captured by value lose their constness
double operation_mutable(double a)
{
	double pi{3.14}, e{2.71}, result;
	
	//capture variables defined in the same scope as the lambda variable
	//make the lambda mutable, so it allows to change the values of captured variables within lambda, but the changes do not persist afterwards
	//by default captured variables are const, but mutable changes their constness
	auto lambda([pi, e](double in) mutable
				{
					e-=1.11;
					pi*=2.1;
					cout<<"inside mutable lambda->new e="<<e<<" new pi="<<pi<<endl;
					return in*pi + e;
				});
	
	result = lambda(a);
	cout<<"outside mutable lambda->new e="<<e<<" pi="<<pi<<endl;
	
	return result;
}

double operation_def_captures(double a)
{
	double pi{3.14}, e{2.71}, result;
	
	//capture variables defined in the same scope as the lambda variable
	//all variables within lambda, which are not parameters, are captured by ref
	auto lambda([&](double in)
				{
					e-=1.11;
					pi*=2.1;
					cout<<"inside def capturing by ref lambda->new e="<<e<<" new pi="<<pi<<endl;
					return in*pi + e;
				});
	
	result = lambda(a);
	cout<<"outside def capturing by ref lambda->new e="<<e<<" new pi="<<pi<<endl;
	
	return result;
}

int main()
{
	//lambda variable; use copy construction. Uniform initialization does not work
	//lambdas do not have a type. The compiler generates a special unique type for lambdas which is not exposed.
	//the return type for the lambda below is deducted from its internal return statement
	auto boolToWord ([](bool in){ return (in) ? "true" : "false";});
	unique_ptr<int> uptr;
	cout<<boolToWord(uptr==nullptr)<<endl;
	
	//pointer to func
	const char* (*msg)(int) = [](int i){ return (i>0)?"positive":"negative";};
	cout<<msg(42)<<endl;
	
	//std::function with copy construction
	function<int(char)> func_ascii( [](char c){return c;} );
	cout<<func_ascii('a')<<" "<<func_ascii('A')<<endl;
	
	//call function whose arg is a lambda defined previously
	func(boolToWord, true);
	//call function with a lambda treated as an anonymous object
	func([](bool in){return (in)?"T":"F";}, false);
	
	
	cout<<operation(-1.23)<<endl;
	cout<<operation_mutable(3.67)<<endl;
	cout<<operation_def_captures(2.2)<<endl;
	
	return 0;
}
