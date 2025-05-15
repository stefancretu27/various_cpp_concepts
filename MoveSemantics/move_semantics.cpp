//for size_t
#include <cstdio>
//for move
#include <utility>
#include <iostream>
#include <vector>

#include "Base.hpp"
#include "ComposeFrom.hpp"
#include "MoveClass.hpp"

using namespace std;

template<typename T>
class MyArray: public Base
{
	private:
		size_t mLength;
		T* mBuffer;
		ComposeFrom arrayName;
		
	public:
		MyArray(size_t length, const ComposeFrom& n, const string& _label):Base{_label}, mLength{length}, mBuffer{new T[length]}, arrayName{n.getName()}
		//also arrayName{n} works, but it calls copy c-tor instead of c-tor. Same for Base, if a base argument would have been passed to MyArray c-tor
		{
			cout<<"MyArray c-tor"<<endl;
		}

		~MyArray(){delete mBuffer;};
		
		//Base copy c-tor is not implicitly called
		//implement copy semantics: Base members cannot be directly initialzied in Derived member initializer list
		MyArray(MyArray &obj):Base{obj}, mLength{obj.mLength}, arrayName{obj.arrayName}
		{
			//allocate memory for the copy's buffer
			mBuffer = new T[mLength];
			
			//deep copy
			for(size_t i{0}; i < mLength; ++i)
			{
				mBuffer[i] = obj.mBuffer[i];
			}
			
			cout<<"MyArray copy c-tor"<<endl;
		};

		MyArray& operator=(MyArray &obj)
		{
			if(this==&obj)
				return *this;
			
			delete [] mBuffer;
			
			mLength = obj.mLength;
			mBuffer = new T[mLength];
			
			//deep copy
			for(size_t i{0}; i < mLength; ++i)
				mBuffer[i] = obj.mBuffer[i];

			//Base copy assignment is not implicitly called
			//explicit call assignment operator for Base => pass Derived object as argument and the assignment will be performed on Base members via object slicing
			//Base::operator=(obj);
			label = obj.label;
			//call assignment operator for ComposeFrom type
			arrayName = obj.arrayName;

			cout<<"MyArray assignment operator"<<endl;
				
			return *this;
		}
		
		//implement move semantics, explicitly call move c-tor for ComposeFrom types and for inherrited types
		MyArray(MyArray &&obj):Base{move(obj)}, mLength(obj.mLength), arrayName{move(obj.arrayName)}
		{
			//do not allocate separate memory and copy data as in copy c-tor, but set the this->mBuffer pointer to point to the same memory location as obj.mBuffer
			mBuffer = obj.mBuffer;
			//now the buffer is pointed to only by this->mBuffer, as obj.mBuffer will point to null
			obj.mBuffer = nullptr;
			obj.mLength = 0;

			cout<<"MyArray move c-tor"<<endl;
		}
		
		MyArray& operator=(MyArray &&obj)
		{
			if(this==&obj)
				return *this;
				
			//firstly, delete the container pointed to by this->mBuffer
			delete [] mBuffer;
			
			//set the this->mBuffer pointer to point to the same memory location as obj.mBuffer
			mLength = obj.mLength;
			mBuffer = obj.mBuffer;

			//explicitly call Base and ComposeFrom move assignment operators, as they are not automatically called as it happens with copy assignment operators
			//Base::operator=(move(obj));
			label = move(obj.label);
			arrayName = move(obj.arrayName);
			//reset obj.mBuffer to point to null =>leave both this and obj in a well defined state
			obj.mLength = 0;
			obj.mBuffer = nullptr;
				
			cout<<"MyArray move assignment operator"<<endl;

			return *this;
		}
		
		size_t getLength(){return mLength;};
		void setlength(size_t length){mLength = length;};
		T& operator[](size_t index){return mBuffer[index];};
		ComposeFrom getArrayName() const {return arrayName;};
		string getLabel() const {return label;};
};

int main()
{
	cout<<"create Base, ComposeFrom and Derived objects"<<endl;
	string label{"labelled"};
	ComposeFrom arrayName{"first array"};
	MyArray<int> arr(3, arrayName, label);
	arr[0] = 0;
	arr[1] = 1;
	arr[2] = 2;
	
	cout<<endl<<"Create copy_arr from Derived object using copy c-tor."<<endl;
	MyArray<int> copy_arr(arr);
	cout<<" arr length: "<<arr.getLength()<<" copy_arr length: "<<copy_arr.getLength()<<endl;
	for(size_t x{0}; x<copy_arr.getLength(); ++x)
		cout<<copy_arr[x]<<" ";
	cout<<endl<<copy_arr.getArrayName().getName()<<" "<<copy_arr.getLabel()<<endl;

	cout<<endl<<"Create move_arr from Derived object using move c-tor."<<endl;
	MyArray<int> move_arr(move(arr));
	cout<<" arr length: "<<arr.getLength()<<" copy_arr length: "<<move_arr.getLength()<<endl;
	for(size_t x{0}; x<move_arr.getLength(); ++x)
		cout<<move_arr[x]<<" ";
	cout<<endl<<move_arr.getArrayName().getName()<<" "<<move_arr.getLabel()<<endl;
	
	cout<<endl<<"Recreate arr using move assignment. arr length: "<<arr.getLength()<<" copy_arr length: "<<copy_arr.getLength()<<endl;
	arr = std::move(copy_arr);
	for(size_t x{0}; x<arr.getLength(); x++)
		cout<<arr[x]<<" ";
	cout<<endl<<arr.getArrayName().getName()<<" "<<arr.getLabel()<<endl;
	
	//use copy semantics
	cout<<endl<<"recreate copy_arr using copy assignment. arr length: "<<arr.getLength()<<" copy_arr length: "<<copy_arr.getLength()<<endl;
	copy_arr = arr;
	for(size_t x{0}; x<copy_arr.getLength(); ++x)
		cout<<copy_arr[x]<<" ";
	cout<<endl<<copy_arr.getArrayName().getName()<<" "<<copy_arr.getLabel()<<endl;

	MyArray<int>&& rref_arr{move(arr)};
	cout<<endl<<"create ref to r-value arr using move arr length: "<<arr.getLength()<<" copy_arr length: "<<rref_arr.getLength()<<endl;

	cout<<endl<<"Conclusions on move semantics implementation + inheritance + composition"<<endl;
	cout<<"     1. Move semantics is implicitly generated by the compiler, for a given class, if all non-static members and all base classes are movable (they have move semantics implemented) and if"<<endl;
	cout<<"     the class implementation doesn't involve an explicit definition for either of the following: copy semantics, destructor. If any of the latter is implemented, the compiler assumes the resources"<<endl;
	cout<<"     lifetime and ownership is manually managed (cleanup, copy and move), as a consequence of programmer's choice of design (if one wants to customly cleanup a resource, might want to copy/move it)."<<endl;
	cout<<"     However, defining an implementation for the c-tor (eventually with parameters) doesn't suprres the generation of implicit move semantics, if all members are movable."<<endl;<<endl;
    	cout<<"     3. Usually, the implementation involves assigning/passing/moving the resources from the argument object to the newly created or assigned to object. This means the object received as argument "<<endl;
    	cout<<"     does not hold these resources anymore. In other words, the old object is left in a well defined state, with all its attributes and resources reset to default or null, either deallocated or closed."<<endl<<endl;
	cout<<"     This is referred to as deep move. In contrast with it, the shallow move, used by the implicitly generated move semantics implementation, performs as shallow move, or rather a member-wise call to std::move."<<endl;
	cout<<"     which, consequently, invokes the move semantics of each member, if any, otherwise falls back to copy semantics. Whe it comes to raw pointer mmebers, the shallow move behaves like shallow copy."<<endl<<endl;
	cout<<"     4. When dealing with move semantics for a class that is composed of other types, its move semantics implementation will simply call std::move for "<<endl;
    	cout<<"     the attributes it is composed of. This means the classes that it is composed from must implement their own move semantics which is invoked by a std::move call"<<endl;
	cout<<"     on their instances."<<endl<<endl;
	cout<<"     5. When dealing with move semantics and inheritance, the move c-tor and move assignment from the Base class must be explicitly called in Derived's' class"<<endl;
	cout<<"     move semantics implementation, with the passed argument being a std::move call on the argument passed to Derived move c-tor or move assignment operator."<<endl;
	cout<<"     Derived(Derived&& d):Base{std::move(d)} and Base::operator=(move(d)) or baseAttribute = std::move(d.baseAttribute)."<<endl<<endl;

	cout<<endl<<"---------------auto, auto& and auto&& + move-------------------"<<endl;
	MoveClass up_MoveClass{};
	//MoveClass& refInst_MoveClass{up_MoveClass};
	
    auto copy_MoveClass{up_MoveClass};
    cout<<"Create auto copy from existing instance => copy c-tor call "<<up_MoveClass.intVec.size()<<" "<<copy_MoveClass.intVec.size()<<endl;

	auto& ref_MoveClass{up_MoveClass};
    cout<<"Create auto ref to existing instance => no copy c-tor call, as ref creates a pointer to existing instance:  "<<up_MoveClass.intVec.size()<<" "<<ref_MoveClass.intVec.size()<<endl;

    auto copyMove_MoveClass{move(up_MoveClass)};
    cout<<"Create auto copy to ref to r-value by calling move=>  move c-tor call "<<up_MoveClass.intVec.size()<<" "<<copyMove_MoveClass.intVec.size()<<endl;

	auto&& rref_MoveClass{up_MoveClass};
	cout<<"Create auto ref to r-value without invoking move=> no move c-tor/copy c-tor call, as r-value ref creates a pointer to existing instance "<<copyMove_MoveClass.intVec.size()<<" "<<rref_MoveClass.intVec.size()<<endl;

    auto&& rrefMove_MoveClass{move(copyMove_MoveClass)};
    cout<<"Create auto ref to r-value invoking move => no move c-tor call, as r-value ref creates a pointer to existing instance "<<copyMove_MoveClass.intVec.size()<<" "<<rrefMove_MoveClass.intVec.size()<<endl;

	//MoveClass&& rref_MoveClass2{up_MoveClass};
	//const MoveClass& refConst_MoveClass{up_MoveClass};
	//MoveClass&& rref_MoveClass2{refConst_MoveClass};
	cout<<"Create auto ref to r-value from l-value without invoking move=> impossible as it can only refer r-values unlike auto&& which can refer to l-values, r-values, const l-values"<<endl;

	vector<int> vec{1,2,3,4};
	func(move(vec));
	cout<<"After func call vec size(): "<<vec.size()<<endl;

	cout<<endl<<"Conclusions on auto and move semantics"<<endl;
        cout<<"     1. only auto and auto&& can be used to store the output of std::move"<<endl;
        cout<<"     2. auto&& or T&& do not involve creating a new object, as the goal of creating a reference to r-value is to avoid the move of the resources. Instead, they are used"<<endl;
        cout<<"     to retain r-values, which can be literals or returned by calls to std::move. No copy or move c-tor is involved when creating such references to r-values. "<<endl;
	cout<<"     Otherwise, if we recall signature of a move c-tor, it would mean that the move c-tor has to call itself when it is defined."<<endl;

	cout<<"     3. auto& and T& do not involve creating a new object, nor they can store the output of move, as they do not retain references to r-values. Thus, no call copy c-tor"<<endl;
	cout<<"     is involved when refering to an object, as the goal of creating a reference is to avoid copy of the resources. Likewise the move c-tor case, if the copy ct-or would "<<endl;
	cout<<"     have been called when creating a reference it would need to call itself when it is defined - recall here the copy c-tor signature taking reference to const input. "<<endl;
	
	cout<<"     4. auto involves creating a new object. Therefore, it always involves calling either move c-tor or copy c-tor Thus, when calling move to store an object in auto,"<<endl;
        cout<<"     the object is moved, with all its resources assigned to the newly created object. Thereafter, its resources are left in a well defined state (reset to null, "<<endl;
	cout<<"     closed, deallocated). When refring to an object that is not moved, then the copy c-tor is called."<<endl;

	cout<<"     5. MyType&& can only refer to r-values, so a move call is required. It cannot be created from l-values, even if they are stored in references to const, as"<<endl;
	cout<<"     the ref to const is an l-value itself, witht he ability to store r-value."<<endl;
	
	return 0;
}
