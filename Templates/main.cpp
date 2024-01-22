//for size_t
#include <cstdio>
#include <list>
#include <memory>
#include <vector>

// class hierarchy used in code examples
#include "MultipleInheritance.hpp"
#include "StaticPolymorphism.hpp"
#include "TemplateFunctions.hpp"
#include "TemplateMethods/TemplateMethods.hpp"
#include "TemplateClasses/RecursiveVariadicTemplates.hpp"
#include "TemplateClasses/PartialSpecializationPointersRefs.hpp"
#include "TemplateClasses/TemplateClass.hpp"
#include "TemplateTemplateParameter.hpp"
#include "EnableIf.hpp"

// base template struct declaration 
template<class T, class S>
struct Example
{
    T data1;
    S data2;
};

// alias template for the above base template declaration
template<class T>
using ExampleInt = Example<T, int>;

//perfect forwarding example
struct Data
{
    string name;
    const int number;
    
    Data(string&& s, const int i):name{s}, number{i}{};
};

//perfect forwarding implementation for the above
template<class ... Args>
Data factoryPerfectFwd(Args&& ... args)
{
	return Data(forward<Args>(args)...);	
}

int main()
{
	cout<<endl<<"Insights on template concepts:"<<endl;
	cout<<"     1. In C++11 template is an entity defining a family of functions/methods or a family of classes. "<<endl;
	cout<<"     A template declaration starts with 'template' word followed by a list of parameters, specified between '<' and '>'. It is mandatory "<<endl;
	cout<<"     to have at least one parameter, for the template declaration to be correct. Otherwise, there is no point in defining the template."<<endl;
	
	cout<<"     2. A template can be parameterized by one or more parameters, that can have the following kinds:"<<endl;
	cout<<"     	i) template type parameter: it acts as a placeholder for a data type. The data type can be a fundamental one or defined"<<endl;
	cout<<"     	as a class/struct. It has a generic name, usually is T, that is preceded by keyword 'class' or 'typename', as it is no difference"<<endl;
	cout<<"     	between 'class' and 'typename' keywords in a template parameter list. Then, in class/function implementation the generic name is used"<<endl;
	cout<<"     	as the data type placeholder for some variables. Hence, the idea of family of classes/functions, as the same implementation can be"<<endl;
	cout<<"     	reused for several distinct data types, that are explicitly specified as arguments upon class instantiation/function invokation"<<endl;
	
	cout<<"     	ii) template non type parameter: it explicitly specifies a data type that is used by all classes/function in the same family."<<endl;
	cout<<"     	The accepted types are restricted to:"<<endl;
	cout<<"     		-> integral types (bool, char, int with their short/long variations and cv qualifications)"<<endl;
	cout<<"     		-> enumerator of an enum"<<endl;
	cout<<"     		-> pointers to objects/functions, including nullptr"<<endl;
	cout<<"     		-> pointers to data members or methods of a class/struct, including nullptr"<<endl;
	cout<<"     		-> reference to lvalues (&) to objects/functions"<<endl;
	
	cout<<"     	iii) template template parameter: template classes, but not functions, can be template parameters for another template's definition. Here, "<<endl;
	cout<<"     	the parameters of the template template parameter are mandatory specified by keyword 'typename' with their name not necessarily provided."<<endl;
	cout<<"     	eg: template <typename T, template <typename, typename> class Cont >"<<endl;
	cout<<"     	    class Matrix{"<<endl;
	cout<<"     		Cont<T, std::allocator<T>> data; ->the template template parameter used the template type parameter."<<endl;
	cout<<"     	    };"<<endl<<endl;
	
	//call template function. with the 2 template template arguments being just the name of the structs/classes
    	function<int, Functor, FunctionWrapper>(8);
    	//call template class overloaded function call operator that wraps the functor
    	MyClass<int, Functor>()(-27, 3, 11);
	
	cout<<"     3. When template arguments are provided upon function invokation or class instantiation, a specialization for that template is "<<endl;
	cout<<"     implicitly generated, with the specified types substituting the template parameters. For functions, the types can be deduced from "<<endl;
	cout<<"	    function's argument types."<<endl;
	cout<<"     Specializations can also be provided explicitly, as specific behaviors can be implemented for some specific types. A full"<<endl;
	cout<<"     specialization entails all template parameters are explicitly stated upon definition. It is supported both by classes and functions."<<endl;
	cout<<"     Partial specialization means that some template parameters, at least one, still is generic. A specialization for pointer/ref to the same "<<endl;
	cout<<"     generic type as the base template, is considered a partial specialization. It is supported only by classes/structs"<<endl<<endl;
	
	cout<<"     4. When a class/function template specialization, is referenced in a context, that template is instantiated with the types provided "<<endl;
	cout<<"     as arguments. Thus, it is generated a complete function definition (lvalue) or a complete object definition (for the template class instance)."<<endl;
	cout<<"	    When it comes to template classes, their methods are not instantied unless used/called."<<endl;
	cout<<"     The instantiation step requires that template definition/implementation to be visible. Oftentimes, template declarations and definitions"<<endl;
	cout<<"     are entirely included in header files. If the code is consdiered too verbose, the implementations can reside in source code files, with"<<endl;
	cout<<"     the instantiations either specified at the end of this file, or in a separate source code file including the source code file with "<<endl;
	cout<<"     the implementations."<<endl<<endl;
	
	cout<<"     5. The aliasing syntax from C++, which binds an identifier to a name (using name = identifier), can also be applied to template declarations,"<<endl;
	cout<<"     leading to template aliases. This is useful in cases a template has more parameters and there is envisaged to provide some specific names"<<endl;
	cout<<"	    for declarations of partial specializations. Consequently, the provided name will still be templated."<<endl<<endl;
	
	// instantiate base template
	Example<char, double> inst{'a', 2.7182};
	// instantiate template alias
    	ExampleInt<bool> instInt{false, 44};
	
	cout<<"     6. Polymorphism represents the capability of different entities to support the same interface. In other terms, the same interface can be"<<endl;
	cout<<"     used (called) by entities of different types. The interface can encapsulate the same behavior for all types, as in the case of"<<endl;
	cout<<"	    template functions, or a distinct behavior particular to each type, as in the case of overloading and overriding."<<endl;
	cout<<"	    There are 2 types of polymoprhism: static and dynamic. The static polymorphism is achieved at compile time either using overloading"<<endl;
	cout<<"	    or templates. As templates can take template type parameters as placeholders for types/entities they work with, the encapsulated"<<endl;
	cout<<"	    behavior becomes available to those entities, which use/invoke the (same) template interface."<<endl<<endl;
	
	//dynamic polymoprhism example
	list<unique_ptr<Interface>> inheritanceChainRefs;
	inheritanceChainRefs.push_back(make_unique<Derived2>());
	inheritanceChainRefs.push_back(make_unique<Derived1>());
	inheritanceChainRefs.push_back(make_unique<Derived12>());

	for(auto&& inst : inheritanceChainRefs)
	{
	    inst->printName();
	}

	// changed into static polymorphic implementation
	printNameFunc(Base1{});
	printNameFunc(Base2{});
	printNameFunc(DerivedBase12{});
	
	cout<<"     7. Variadic templates are template functions/classes which can take an arbitrary number of arguments."<<endl;
	cout<<"     Thus, variadic templates are to be used when the function/class needs a variable number of arguments of a variety of types "<<endl;
	cout<<"     This behavior is achieved by using ellipsis operator (...) which makes Args a parameter pack (distinct parameters, "<<endl;
	cout<<"	    possibly of different types, that are tied together). Parameter packs can be subjects to 2 operations only: they can be packed or"<<endl;
	cout<<"	    unpacked. When the ellipsis precedes Args, they are packed; when the 3 dots are on the right of Args, they are unpacked."<<endl;
	
	cout<<"	    The main usage of this syntax is the recursion applied to templates. In detail, an operation is performed on the first argument,"<<endl;
	cout<<"	    then the function is called recursively for the parameter pack, sent as argument with the unpacking syntax (arg...)."<<endl;
	cout<<"	    Thus, the parameter pack is reduced at each recursive call. Furthermore, ther should be implemented an explicit specialization "<<endl;
	cout<<"	    for 1 template argument, or for none, which would be called in th last instance, acting as stop condition for the recursive calls."<<endl;
	cout<<"	    Under the hood, the compiler generates an implicit specialization for each template function called in the recursive call graph. So,"<<endl;
	cout<<"	    another template function definition is generated, at each recursive step, with the new set of arguments resulted from unpacking."<<endl<<endl;
	
	cout<<VariadicArgsExample<int, int, int, int>::computeSum(1, 2, 3, 4)<<endl;
    	cout<<VariadicArgsExample<char, int, float, bool>::computeSum('c', 1, 3.14159, false)<<endl;
    	cout<<VariadicArgsExample<int, char, float, bool, short int, unsigned long long>::computeSum(1, 'c', 3.14159, false, -11, 11)<<endl;
	
	myPrintf("\n");
    	myPrintf("% world% %\n", "Hello", '!', 2022); 
	
	cout<<"	    8. Perfect forwarding is used often in conjunction with variadic templates. It allows to preserve the argument's value category,"<<endl;
	cout<<"	    be it lvalue or rvalue, but also its cv-qualification. It is useful when the template function need to pass the received arguments,"<<endl;
	cout<<"	    without changing them, to another function that it wraps =>forwarding. A rvalue argument to the wrapper would become a lvalue argument"<<endl;
	cout<<"	    for the wrapped function call (unless an explicit std::move call is made)."<<endl;
	
	cout<<"	    The pattern to implement perfect forwarding consists fo 3 steps"<<endl;
	cout<<"	    	i) use  atemplate definition: template<class T>/template<class ... Args>"<<endl;
	cout<<"	    	ii) use universal reference for the argument such that lvalue/rvalue category is preserved: myFunc(T&& arg)/myFunc(Args&&... args)"<<endl;
	cout<<"	    	iii) invoke std::forward on the argument: std::forward<T>(arg)/ std::forward<Args>(args)..."<<endl;
	
	//template functions with specializations and overloads for smart pointers
	static constexpr double pi{3.14159};
	int i{2};
	float f = castTtoS<int, float>(i);
	cout<<"result of call to template method that performs static_cast: "<<f*pi<<endl;

	shared_ptr<Base2> spBase2{make_shared<Derived>()};
	//perform side cast
	shared_ptr<Base3> spBase3 = castTtoS<Base2, Base3>(spBase2);
	spBase3->printName();
	spBase3->printString();

	cout<<is_polymorphic<Base2>::value<<endl;

	shared_ptr<Derived> spDerived = castTtoS<Base2, Derived>(spBase2);
	spDerived->printName();
	spDerived->printString();

	//template functions and methods----------------------------------------------------------------------------------------------------
	double d{3.14159};
	wrapTemplateMethodCall<double>(d);
	wrapTemplateMethodCall<bool>(true);

	unsigned* ptrUInt = new unsigned{9};
	wrapTemplateMethodCall<unsigned*>(ptrUInt);

	void* voidPtr = new long{123456789};
	wrapTemplateMethodCall<void*>(voidPtr);

	NonTemplateClass tempMethodsInst{27};
	tempMethodsInst.doWork<double>(3.14159);
	tempMethodsInst.doWork(true);
	tempMethodsInst.doWork<long>(123456789);
	
	tempMethodsInst.doWork<double, float>(3.14159, 2.7182);
	tempMethodsInst.doWork<bool, bool>(true, false);

	tempMethodsInst.doWork<double, bool>(3.14159, true);
	tempMethodsInst.doWork<bool, float>(false, 2.7182);

	//template class-------------------------------------------------------------------------------------------------------------------
	
	std::cout<<"Use partial specialization for pointers and references in template classes to check a data type is plain, pointer or reference"<<std::endl; 
	double d{2.7182};
        double* ptrD{new double(-273.1)};
        double& refD{d};
    
        cout<<endl<<"IsPlainData "<<CheckType<double>::IsPlainData(d)<<" IsPtr "<<CheckType<double>::IsRawPointer(d)<<" IsRef "<<CheckType<double>::IsRef(d)<<endl;
        cout<<"IsPlainData "<<CheckType<double*>::IsPlainData(ptrD)<<" IsPtr "<<CheckType<double*>::IsRawPointer(ptrD)<<" IsRef "<<CheckType<double*>::IsRef(ptrD)<<endl;
        cout<<"IsPlainData "<<CheckType<double&>::IsPlainData(refD)<<" IsPtr "<<CheckType<double&>::IsRawPointer(refD)<<" IsRef "<<CheckType<double&>::IsRef(refD)<<endl;

	std::cout<<"Use copy-and-swap idiom in implementation of template class with 2 template parameters"<<std::endl; 
	TemplateClass<int, double> tc{-42, 3.14159}, tc2{89, 2.7182}, tc3{};
        cout<<" tc "<<tc.GetMMember()<<" "<<*tc.GetMUptrData()<<endl;
        tc3 = tc2;
        cout<<"tc2 "<<tc2.GetMMember()<<" "<<*tc2.GetMUptrData()<<" tc3 "<<tc3.GetMMember()<<" "<<*tc3.GetMUptrData()<<endl;
        tc2 = move(tc);
        cout<<"tc2 "<<tc2.GetMMember()<<" "<<*tc2.GetMUptrData()<<" tc "<<tc.GetMMember()<<" "<<(tc.GetMUptrData()==nullptr)<<endl;

	std::cout<<"Use variadic template arguments to recursively print data arguments. The print method is encapsulated in a struct, thus allowing for partial specializations"<<std::endl; 
	DataPrinter<int, double, char, string>::PrintData(-273, 2.7182, 'c', "string");
    	ArgsTuple<4, string, double, int, char>::PrintArgs("<variadic args to tuple>", 3.1415, -71, 'k');
    	ArgsTuple<4, tuple<double, char, string, int>>::PrintArgs(make_tuple(20.202, 'y', "<<args with tuple>>", 27));

	IntegersWrapper<int> iw{5};
        iw.PrintData();
	IntegersWrapper<char> iwc{'c'};
	iwc.PrintData();
	iwc.PrintType();
	//compile time error if the class is instantiated with another type than integral, as below
	//IntegersWrapper<const char*> iwcstr{"C-style string"};
	//iwcstr.PrintData();
	//IntegersWrapper<double> iwf{3.14159};
	//iwf.PrintData();
	DoubleWrapper<double> dwd{3.14159};
	dwd.PrintData();
	    
	EnableViaReturnType<float>(2.7182);
	EnableViaReturnType<const char*>("C-style string");
	//compile time error if the function is instantiated with a non trivial type
	//EnableViaReturnType<std::string>("std string");
	EnableViaFunctionParameter<float>(2.7182);
	
	return 0;
}
