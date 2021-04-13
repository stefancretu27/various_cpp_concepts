//for size_t
#include <cstdio>
//for move
#include <utility>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Base
{
	protected:
	string label;

	public:
	Base() = default;
	Base(const std::string str):label{str}
	{
		cout<<"Base c-tor"<<endl;
	};
	Base(const Base& n):label{n.label}
	{
		cout<<"Base copy c-tor"<<endl;
	};
	Base(Base&& n):label{move(n.label)}
	{
		cout<<"Base move c-tor"<<endl;
	}

	Base& operator=(const Base& n)
	{
		if(this == &n)
			return *this;

		label = n.label;

		cout<<"Base assignment operator"<<endl;

		return *this;
	}

	string getLabel() const {return label;};
};

class Nested
{
	private:
	string name;

	public:
	Nested() = default;
	Nested(const std::string str):name{str}
	{
		cout<<"Nested c-tor"<<endl;
	};
	Nested(const Nested& n):name{n.name}
	{
		cout<<"Nested copy c-tor"<<endl;
	};
	Nested(Nested&& n):name{move(n.name)}
	{
		cout<<"Nested move c-tor"<<endl;
	}

	Nested& operator=(const Nested& n)
	{
		if(this == &n)
			return *this;

		name = n.name;

		cout<<"Nested assignment operator"<<endl;

		return *this;
	}

	string getName() const {return name;};
};

template<typename T>
class MyArray: public Base
{
	private:
		size_t mLength;
		T* mBuffer;
		Nested arrayName;
		
	public:
		MyArray(size_t length, const Nested& n, const string& _label):Base{_label}, mLength{length}, mBuffer{new T[length]}, arrayName{n.getName()}
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
			Base::operator=(obj);
			//call assignment operator for Nested type
			arrayName = obj.arrayName;

			cout<<"MyArray assignment operator"<<endl;
				
			return *this;
		}
		
		//implement move semantics, explicitly call move c-tor for nested types and for inherrited types
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

			//explicitly call Base and Nested move assignment operators, as they are not automatically called as it happens with copy assignment operators
			Base::operator=(move(obj));
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
		Nested getArrayName() const {return arrayName;};
		string getLabel() const {return label;};
};

class MoveClass
{
    public:
    std::vector<int> intVec;

    public:
    MoveClass()
    {
        intVec.push_back(1);
        intVec.push_back(2);
        intVec.push_back(3);
        std::cout<<"C-tor"<<std::endl;
    };
    MoveClass(const MoveClass& inst)
    {
        intVec = inst.intVec;
        std::cout<<"Copy c-tor"<<std::endl;
    };
    MoveClass(MoveClass&& inst)
    {
        intVec = std::move(inst.intVec);
        std::cout<<"Move c-tor"<<std::endl;
    };
    MoveClass& operator=(const MoveClass& inst){std::cout<<"Copy assignment"<<std::endl; return *this;};
    MoveClass& operator=(MoveClass&& inst){std::cout<<"Move assignment"<<std::endl; return *this;};

};

void func(std::vector<int>&& vec)
{
	cout<<"In function moved vec size(): "<<vec.size()<<endl;
}

int main()
{
	cout<<"create Base, Nested and Derived objects"<<endl;
	string label{"labelled"};
	Nested arrayName{"first array"};
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

	cout<<"---------------auto, auto& and auto&& + move-------------------"<<endl;
	MoveClass up_MoveClass{};
	//MoveClass& refInst_MoveClass{up_MoveClass};
	
    auto copy_MoveClass{up_MoveClass};
    cout<<"Create a copy from existing instance => copy c-tor call "<<up_MoveClass.intVec.size()<<" "<<copy_MoveClass.intVec.size()<<endl;

	auto& ref_MoveClass{up_MoveClass};
    cout<<"Create ref to existing instance => no copy c-tor call, as ref creates a pointer to existing instance:  "<<up_MoveClass.intVec.size()<<" "<<ref_MoveClass.intVec.size()<<endl;

    auto copyMove_MoveClass{move(up_MoveClass)};
    cout<<"Create copy to ref to r-value =>  move c-tor call "<<up_MoveClass.intVec.size()<<" "<<copyMove_MoveClass.intVec.size()<<endl;

    auto&& rref_MoveClass{move(copyMove_MoveClass)};
    cout<<"Create copy to ref to r-value => no move c-tor call, as r-value ref creates a pointer to existing instance "<<copyMove_MoveClass.intVec.size()<<" "<<rref_MoveClass.intVec.size()<<endl;

	vector<int> vec{1,2,3,4};
	func(move(vec));
	cout<<"After func call vec size(): "<<vec.size()<<endl;
	
	return 0;
}
