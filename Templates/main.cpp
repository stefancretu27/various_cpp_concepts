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


//variadic template struct
template<class T, class ... Args>
struct VariadicArgsExample
{
    static T computeSum(T firstValue, Args... args)
    {
        if(is_fundamental<T>::value)
        {
            return firstValue + VariadicArgsExample<Args...>::computeSum(args...);
        }
    }
};

//partial specialization with 1 argument for the struct above 
template<class T>
struct VariadicArgsExample<T>
{
    static T computeSum(T singleValue)
    {
        return singleValue;
    }
};


//example of a printf using variadic templates and cout
void myPrintf(const char* format)
{
    cout<<*format;
}

template<class T, class ... Args>
void myPrintf(const char* format, T firstValue, Args ... args)
{
    for(; *format!='\0'; ++format)
    {
        //if a format specifier is encountered, the actual type specifier is omitted
        //as cout knows how to interpret them
        if(*format == '%')
        {
            //cout the input value and recursively call with the format poinitng
            //to the next character and with expanded pack
            cout<<firstValue;
            myPrintf(format + 1, args...);
            break;
        }
        else
        {
            cout<<*format;
        }
    }
}


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
	cout<<"     This behavior is achieved by using ellipsis operator (...) which makes Args or args a parameter pack (distinct parameters, "<<endl;
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

	cout<<endl<<"Insights on template functions and methods:"<<endl;
	cout<<"     1. The template functions represent a pattern, that uses placeholder for generic types, which is used to create similar functions, with "<<endl;
	cout<<"     specific data types. Thus, the same piece of code is reused in distinct contextes with distinct data types, that should fit in the "<<endl;
	cout<<"     function's logic (eg: does not make sense to pass char or string data types to a template function performing some numeric operations)."<<endl<<endl;

	cout<<"     2. A template function's definition includes a template parameter declaration which specifies that some placeholders are to be used"<<endl;
	cout<<"     for the data types that are supposed to be generic. This generic declaration is not compiled directly, as the compiler does not know how to"<<endl;
	cout<<"	    compile generic template parameter types Instead, each time it is invoked, the data types for the generic types are explicitly specified,"<<endl;
	cout<<"     thus the compiler replicates the generic pattern and creates a function with the actual data types."<<endl;
	cout<<"     In other terms, the compiler stencils out a copy of the generic definition, replaces the template type parameters"<<endl;
	cout<<"     with those provided by that call, creating a specific function, which is referred to as template function instance."<<endl<<endl;

    	cout<<"     3. Like a plain function/method, a template function or method can be overloaded following the same principles. Nevertheless, a template"<<endl;
	cout<<"     specialization does not overload."<<endl;

	cout<<"     4. A template method can be defined inside a non template class. In this case it is recommended that the implementation of the template "<<endl;
	cout<<"     methods should be done in a separate .cpp file from the .cpp file where the non-template methods are implemented. Thus, the said source file"<<endl;		
	cout<<"     can be included in the .cpp file where the templates are intasntiated with the used data types, such that the compiler can resolve the calls"<<endl<<endl;

	cout<<"     5. Template functions cannot be partially specialized, but only fully specialized. Nevertheless, in such cases, as the data types are known"<<endl;
    	cout<<"     it is preferred to implement an overload. That is due to the steps of resolving a function's call:"<<endl<<endl;
	cout<<"     	i) Firstly, non template implementations are looked for. The one that best matches the parameter types is chosen, if any."<<endl;
    	cout<<"     	ii) Secondly, the templates are looked for. The best overload is chosen, if any."<<endl;
	cout<<"     	iii) Lastly, for the most suitable overload, it is looked for a specialization, if any, that best matches the call"<<endl;
	cout<<"     As it can be seen, providing a specialization might lead to situation when it is ignored when resolving a call, if there is an overload"<<endl;
	cout<<"     that is considered to be a good fit, because it depends for which overload that specialization was implemented."<<endl;
	cout<<"     On top of that template method overloading should be preferred to explicit specialization for one more reason: template methods caanot"<<endl;
	cout<<"     be fully specialized within a class, unless the class is specialzied itself."<<endl<<endl;

	cout<<"     6. The full specialization of a function/method is also called explicit specialization. When fully specializing a function/method, "<<endl;
    	cout<<"     all the template parameters are omitted from the template declaration (thus, only template<> remains). The specialization entails that"<<endl;
	cout<<"     all data types corresponding to template types are known, therefore they can be directly used when implementing the full specialization"<<endl;
	cout<<"     Optionally, the data types can be inserted after the function's name, between <>, marking its name as templatized (void f<int, float>(args))"<<endl;
	cout<<"     Moreover, the specialization must have the same signature with the template definition it specializes, following similar principles as overriding"<<endl<<endl;
	
	cout<<"     7. When writing a template function, if it is envisaged for specialization, besides overloading, it is recommended to encapsulate it, as a" <<endl;
	cout<<"     static method of astruct/class which can be full or partially specialized. Thereafter, the function shall be called via a wrapper whose "<<endl;	
	cout<<"     signature stays always the same. This wrapper shall not be subject of specialization."<<endl;
	cout<<"     Another interesting aspect here is that when the struct/class encapsulating the static template method is fully or partially specialized"<<endl;
	cout<<"     the method's signature can be slightly changed (arguments types, return type, but not args number) and the specialization stays valid. "<<endl;
	cout<<"     When a function is specialized, its signature should stay the same (return type, argument's types and const qualifiers, its constness)"<<endl;
	
	cout<<"     8. The approach described above can also be applied when specializing template method of a non template class. Nonetheless, the syntax"<<endl;
	cout<<"     requires some adjustments. Firstly, the template struct which wraps the template method should eb defined inside the non template class"<<endl;
	cout<<"     for an easier access to members. Thereafter, for the wrapped method to be able to access members of the outer class, it has to receive as"<<endl;
	cout<<"     argument the instance (this pointer) of the calling instance of the non template class. The wrapped method stays as static and does not"<<endl;
	cout<<"     have a this pointer from the wrapping template struct."<<endl;

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

	cout<<endl<<"Insights on template class:"<<endl;
	cout<<"     1. A template class that is implemented in a .cpp file requires another .cpp file where it is explicitly instantiated with the needed"<<endl;
	cout<<"     data types. As explained above, this need comes from the fact that the template is not compiled by itslef, instead it is replicated when"<<endl;
	cout<<"     an explicit instance is done. Furthermore, the additional .cpp file needs access to the template implementations of the class methods in"<<endl;
	cout<<"     order to properly generate the code for each explicit instantiation. Thus, the .cpp file with template implementations must be included"<<endl;
	cout<<"     in the additional .cpp file that holds the explicit instantiations."<<endl;

	cout<<"     2. A template class can be fully specialized. That is, a separate implementation is provided using explicit data types for all its"<<endl;
	cout<<"     template parameters. In this case, the class declaration starts with template<>, keeping the template parameter list empty, whereas"<<endl;
	cout<<"     the explicit data types are provided after the class name (ClassName<dataType1, dataType2>) when the class is defined and when its methods"<<endl;
	cout<<"     are implemented in a separate file. In addition, when implementing its methods the template<> specifier is not needed anymore. "<<endl;
	cout<<"     Since all its data type are explicitly specified, it is clear that a full specialization does not require explicit instantiation. Its"<<endl;
	cout<<"     syntax is mostly like the one of a plain class."<<endl;

	cout<<"     3. Unlike template functions, a template class can be [partially] specialized. That is, a separate implementation is provided for some"<<endl;
	cout<<"     of its template type parameters. Those parameters that are not specialized, remain general and the syntax for them stays similar with regular"<<endl;
	cout<<"     template classes. For the specialized types, the syntax follows the principles fo full specialization in the sense that the explicit types"<<endl;
	cout<<"     must be included after the class name when defining and and when implementing its methods."<<endl;
	cout<<"     Like a template class beholding general types, a partially specialized class requires explicit intantiation with the needed types for its"<<endl;
	cout<<"     generic template parameters."<<endl;
	cout<<"     Partial specialization can also be done for pointers or refs. Even though there is only one template parameter for which the specialization"<<endl;
	cout<<"     is implemented, it is still considered a partial specialization, as the type is not explicitly specified. In this case, the syntax is similar"<<endl;
	cout<<"     full specializations, only that after the class name is specified the pointer/ref type (ClassName<T*>) when defining the class and when"<<endl;
	cout<<"     implementing its methods."<<endl;

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
	
	return 0;
}
