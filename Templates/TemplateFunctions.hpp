#ifndef TEMPLATEFUNCTIONS_HPP
#define TEMPLATEFUNCTIONS_HPP

#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <type_traits>

using namespace std;

// Template base function with 2 template type parameters
template<typename T, class S>
S castTtoS(const T& t)
{
	S result{};

	result = static_cast<S>(t);

	return result;
}

// Specialization for T=bool and S=bool
template<>
bool castTtoS(const bool& spt)
{
	cout<<"		cannot convert bool to bool"<<endl;
	return false;
}

// Specialization for T=int and S=bool
// <int, bool> is not necessary due to template argument deduction, but can be written for clarity
template<>
bool castTtoS<int, bool>(const int& spt)
{
	cout<<"		cannot convert int to bool"<<endl;
	return false;
}

// do not specialize template functions, but use plain function as overload, as long as data types are known
void castTtoS(bool t)
{
	
}

// Overload base template function for raw pointers. 
// It might look like a partial specialization, but it's not as functions/methods cannot be partially specialized.
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

// Template specialization for the above overload for pointers with types T=bool and S=void
template<>
void* castTtoS(bool* spt)
{
	return nullptr;
}

// Overload base template function for smart pointers. 
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
/*
* As Herb Sutter mentioned in http://www.gotw.ca/publications/mill17.htm, template functions/methods should not
* be specialized, as the specializations are not involved in overload resolution. The overload resolution only
* takes into account non template implementations and base template implementations, including their overloads.
* Additionally, functions/methods cannot be partially specialized.
*
* Consequently, if a suitable match is found during an overload resolution, that match will be used. 
* If that match is specialized, ony then a specialization will be chosen. If there is specialized a base template 
* that is considered not the best match, that specialization won't be chosen.
*
* However, when dealing with template functions/methods for which is desired to implement specialization, the 
* devised strategy entails the folloeing steps
*	1) the business logic of that template function is encapsulated as a static emthod of s struct. This way,
* as the struct can be fully/partially specialized, the static method's implementation can be changed accordingly.
*	2) the template function's interface/sigmnature is not altered, as it might be used by other components
*	3) the template function's implementation only makes a call to the static method encapsulated by the struct
*/

// (1) Base template struct with template type parameter T 
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


// (2) Full specialization of the template struct for T=int. The method's return type can be changed.
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

// (3) Partial specialization for raw pointers of the base template struct. 
// This would be impossible with template functions, but would make use of overloads to overcome this downside.
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

// (4) Full specialization of the base template for pointers to void
template<>
struct EncapsulateStaticTemplateMethod<void*>
{
	static bool methodToBeSpecialized(void* t)
	{
		cout<<"		Template method specialized for void* invoked"<<endl;

		return false;
	}
};


// The wrapper of the static template method which performs invokation on the appropriate specialization
template<class T>
void wrapTemplateMethodCall(const T& t)
{
	// If T=void* the call would be solved to (4). If T is a raw pointer, the call would be solved to (3),
	// and (2) would be called for T=int, as T would be deduced accordingly in each case and the appropriate
	// implementation would be chosen, or generated for when (3) or (1) are considered best matches.
	EncapsulateStaticTemplateMethod<T>::methodToBeSpecialized(t);
}

////////////////////////////////////////////////////////
//demonstrate that function explicit specialization is less flexible than class full specialization
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
