//for size_t
#include <cstdio>
//for move
#include <vector>

#include "TemplateFunctions.hpp"
#include "TemplateMethods/TemplateMethods.hpp"

int main()
{
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

	cout<<endl<<"Insights on template functions:"<<endl;
	cout<<"     1. The template functions represent a pattern, that uses placeholder for generic types, which is used to create similar functions, with "<<endl;
	cout<<"     specific data types. Thus, the same piece of code is reused in distinct contextes with distinct data types, that should fit in the "<<endl;
	cout<<"     function's logic (eg: does not make sense to pass char or string data types to a template function performing some numeric operations)."<<endl<<endl;

	cout<<"     2. A template function's definition includes a template parameter declaration which specifies that some placeholders are to be used"<<endl;
	cout<<"     for the data types that are supposed to be generic. This generic declaration is not compiled directly, as the compiler does not know how to"<<endl;
	cout<<"		compile generic template parameter types Instead, each time it is invoked, the data types for the generic types are explicitly specified,"<<endl;
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
	
	cout<<"     8. AThe approach described above can also be applied when specializing template method of a non template class. Nonetheless, the syntax"<<endl;
	cout<<"     requires some adjustments. Firstly, the template struct which wraps the template method should eb defined inside the non template class"<<endl;
	cout<<"     for an easier access to members. Thereafter, for the wrapped method to eb able to access members of the outer class, it has to receive as"<<endl;
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
	
	// //-----------------------------------------------------Static members---------------------------------------------------------
	// cout<<endl<<"-----------------------------------------------------Static members---------------------------------------------------------"<<endl;

	// cout<<endl<<"Conclusions on static members and methods:"<<endl;
    // cout<<"     1. Static members belong to class and not to instances, so they must be explicitly instantiated in a .cpp in order for them to be assigned to a global memory."<<endl;
	// cout<<"     Static members can be const and are accessible in static and non-static methods."<<endl<<endl;
	
	// cout<<"     2. Static methods cannot be const. Also, they only can access static members, since they lack *this pointer, due to they are not bound to any instance of the class."<<endl;
    // cout<<"     Thus, they are accessible via scope resolution operator, but also can be accessed via in instance, though it would eb counter intuitive"<<endl<<endl;

	// cout<<"     3. A Derived class of a Base class that has static members, does not inherit the static members, but it has access to the same static member. Thus, a static member"<<endl;
    // cout<<"     of a Base class is a shared resource of all classes that inherit from it, not only shared amongst its instances."<<endl<<endl;

	// cout<<"     4. A Derived class inherits all static methods from Base class. Furthermore, the Derived class can override Base class inherited static methods. That said, the same "<<endl;
    // cout<<"     rules for methods' inheritances and override do apply for non static methods and for static methods in the same way."<<endl<<endl;
    

	return 0;
}
