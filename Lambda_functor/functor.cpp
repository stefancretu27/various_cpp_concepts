#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/*
 * A functor is a class that overloads the () operator.
 * When instantiated, the objects look like functions.
 */
 
struct functor
{
	int x;
	
	//c-tor
	functor(int val):x(val){};						//so, the ftor contains state
	int operator()(int y){return x+y;};
};
	
int main()
{
	//instantiate the struct and initialize the contained value to 11
	functor ftor11(11);
	//use the instance like a function to call the overloaded () operator				
	int result = ftor11(8);		
	cout<<result<<endl;
	
	//create another ftor
	functor another_ftor(-23);
	result = another_ftor(103);
	cout<<result<<endl;
	
	vector<int> in{0,1,2,3,4};
	vector<int> out(in.size()), out2(in.size());
	//transform applies a function to in, storing output in out. 
	//The unary function arg is created as an anonymous object of functor
	transform(in.begin(), in.end(), out.begin(), functor(23));
	for(int i:out)
		cout<<i<<" ";
	cout<<endl;
	
	//same as above, but use a previously created functor object, not an anonymous one
	functor ftor3(3);
	transform(in.begin(), in.end(), out2.begin(), ftor3);
	for(int i:out2)
		cout<<i<<" ";
	cout<<endl;
	
	
	return 0;
}
