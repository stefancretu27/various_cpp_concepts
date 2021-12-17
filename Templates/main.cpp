//for size_t
#include <cstdio>
//for move
#include <vector>

#include "TemplateFunctions.hpp"
#include "TemplateMethods/TemplateMethods.hpp"
#include "TemplateClasses/TemplateClass.hpp"

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

	cout<<endl<<"Insights on template functions and methods:"<<endl;
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
	
	cout<<"     8. The approach described above can also be applied when specializing template method of a non template class. Nonetheless, the syntax"<<endl;
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
	cout<<"     of its template type parameters. THos parameters that are not specialized, remain general and the syntax for them stays similar with regular"<<endl;
	cout<<"     template classes. For the specialized types, the syntax follows the principles fo full specialization in the sense that the explicit types"<<endl;
	cout<<"     must be included after the class name when defining and and when implementing its methods."<<endl;
	cout<<"     Like a template class beholding general types, a partially specialized class requires explicit intantiation with the needed types for its"<<endl;
	cout<<"     generic template parameters."<<endl;
	cout<<"     Partial specialization can also be done for pointers or refs. Even though there is only one template parameter for which the specialization"<<endl;
	cout<<"     is implemented, it is still considered a partial specialization, as the type is not explicitly specified. In this case, the syntax is similar"<<endl;
	cout<<"     full specializations, only that after the class name is specified the pointer/ref type (ClassName<T*>) when defining the class and when"<<endl;
	cout<<"     implementing its methods."<<endl;

	CheckType<int&> refType{};
	refType.printType();
	
	CheckType<int> intType{};
	intType.printType();
	
	CheckType<int*> ptrType{};
	ptrType.printType();
	
	CheckType<bool> boolType{};
	boolType.printType();
	
	return 0;
}
