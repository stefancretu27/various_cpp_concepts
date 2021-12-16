#ifndef TEMPLATEFUNCTIONS_HPP
#define TEMPLATEFUNCTIONS_HPP

#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <type_traits>

#include "MultipleInheritance.hpp"
using namespace std;

template<typename T, class S>
S castTtoS(const T& t)
{
	S result{};

	result = static_cast<S>(t);

	return result;
}

//specialization for bool
template<>
bool castTtoS(const bool& spt)
{
	cout<<"		cannot convert bool to bool"<<endl;
	return false;
}

//<int, bool> is not necessary, but can be written for clarity
template<>
bool castTtoS<int, bool>(const int& spt)
{
	cout<<"		cannot convert int to bool"<<endl;
	return false;
}

//do not specialize template functions, but use plain function as overload, as long as data type are known
void castTtoS(bool t)
{
	
}

template<typename T, class S>
S* castTtoS(T* spt)
{
	cout<<"template function overload for pointers"<<endl;
	S* result{};
	if(is_polymorphic<T>::value && is_polymorphic<S>::value )
	{
		try
		{
			result = dynamic_cast<S>(spt);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	else if(is_fundamental<S>::value && is_fundamental<T>::value)
	{
		result = reinterpret_cast<S>(spt);
	}
	else
	{
		result = static_cast<S>(spt);
	}

	return result;
	
}

//template specialization for the above with types bool* and void*
template<>
void* castTtoS(bool* spt)
{
	return nullptr;
}

template<typename T, class S>
shared_ptr<S> castTtoS(const shared_ptr<T>& spt)
{
	cout<<"template function overload for shared pointers"<<endl;
	shared_ptr<S> result{};

	if(is_polymorphic<T>::value && is_polymorphic<S>::value )
	{
		try
		{
			cout<<"dynamic pointer cast"<<endl;
			result = dynamic_pointer_cast<S>(spt);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	else if(is_fundamental<S>::value && is_fundamental<T>::value)
	{
		//since c++17
		cout<<"reinterpret pointer cast"<<endl;
		result = reinterpret_pointer_cast<S>(spt);
	}
	else
	{
		cout<<"static pointer cast"<<endl;
		//result = static_pointer_cast<S>(spt);
	}

	return result;
}


/////////////////////////////////////////////////////////////////

template<class T>
struct EncapsulateStaticTemplateMethod
{
	static void methodToBeSpecialized(const T& t)
	{
		cout<<"		Template method invoked"<<endl;
		if(is_fundamental<T>::value)
			cout<<		t<<endl;
	}
};


//full specialization for int, change method's signature
template<>
struct EncapsulateStaticTemplateMethod<int>
{
	static bool methodToBeSpecialized(int t)
	{
		cout<<"		Template method fully specialized for int invoked"<<endl;
		cout<<		t<<endl;

		return true;
	}
};

//partial specialization for pointers
template<class T>
struct EncapsulateStaticTemplateMethod<T*>
{
	static void methodToBeSpecialized(T* t)
	{
		cout<<"		Template partial specialization for pointers invoked"<<endl;
		if(is_fundamental<T>::value)
			cout<<*t<<endl;
	}
};

template<>
struct EncapsulateStaticTemplateMethod<void*>
{
	static bool methodToBeSpecialized(void* t)
	{
		cout<<"		Template method specialized for void* invoked"<<endl;

		return false;
	}
};

//the wrapper of the static template method which performs invokation on the appropriate specialization
template<class T>
void wrapTemplateMethodCall(const T& t)
{
	EncapsulateStaticTemplateMethod<T>::methodToBeSpecialized(t);
}


//demonstrate that function explicit specialization is less flexible than clas full specialization
template<class T>
void anotherMethodToBeSpecialized(const T& t)
{
	cout<<"		Template method invoked"<<endl;
	if(is_fundamental<T>::value)
		cout<<		t<<endl;
}

//The below function specialization does not work as the arg type does not match with the specialzied template: it is not a ref to const
// no instance of function template "anotherMethodToBeSpecialized" matches the specified type
// template<>
// void anotherMethodToBeSpecialized(int t)
// {
// 		cout<<		t<<endl;
// }

template<>
void anotherMethodToBeSpecialized(const int& t)
{
		cout<<		t<<endl;
}

#endif