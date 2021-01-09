#include  "includes.hpp"

void PointersAndConst()
{
    using namespace std;

    cout<<"Relationships between pointers and const are explained here"<<endl;

    //Declare a int variable and a const int variable. Try to store them in a pointer
    int i1{};
    int *pi1{&i1};
    cout<<"int variable: "<<i1<<" dereferenced pointer to int variable: "<<*pi1<<" double preincrement value via pointer: "<<++++(*pi1)<<endl;
    const int i2{1};
    //int* pi2{&i2}; -> a const int cannot be pointed to by a pointer to non-const => needs pointer to const
    const int* pci2{&i2};
    cout<<"const int variable: "<<i2<<" dereference pointer to const int pointing to const int: "<<*pci2<<endl;
    pci2 = &i1;
    cout<<"int variable: "<<i1<<" dereferenced pointer to const int poiniting to non-const int: "<<*pci2<<endl;

    cout<<endl<<"Conclusions for pointer to const: "<<endl;
    cout<<"     1. Pointer to non-const data type can only point to non-const variables."<<endl;
    cout<<"     2. Pointer to const data type can point both to const and non-const variables and treats them as being const."<<endl;
    cout<<"     3. Pointer to const can be changed to point to another value as the pointer is non-const, only the value it points to is const"<<endl<<endl;


    int* const cpi{&i1};
    cout<<"int variable: "<<i1<<" dereferenced const pointer to int: "<<*cpi<<" increment value via const pointer: "<<++(*cpi)<<endl;
    //int* const cpi2{&i2}; -> a const int cannot be pointed to by a const pointer to non-const => needs const pointer to const
    cout<<endl<<"Conclusions for const pointer: "<<endl;
    cout<<"     1. Can point only to non-const data types."<<endl;
    cout<<"     2. Once set to point to a variable, it cannot be reasigned to point to another variable."<<endl;
    cout<<"     3. The value it points to can be changed via pointer dereferencing as the pointer is const the value is non-const."<<endl<<endl;

    const int* const cpci1{&i1};
    cout<<"int variable: "<<i1<<" dereference const pointer to const: "<<*cpci1<<endl;
    const int* const cpci2{&i2};
    cout<<"const int variable: "<<i2<<" dereference const pointer to const: "<<*cpci2<<endl;

    cout<<endl<<"Conclusions for const pointer to const: "<<endl;
    cout<<"     1. Can point to non-const and const data types and treats them as const =>values cannot be changed via dereferencing"<<endl;
    cout<<"     2. Once set to point to a variable, it cannot be reasigned to point to another variable."<<endl;
    cout<<"     Thus, the constness is applied both to the pointer and also to the value it points to."<<endl<<endl;

}