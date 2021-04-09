//for size_t
#include <cstdio>
//for move
#include <utility>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

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
class MyArray
{
	private:
		size_t mLength;
		T* mBuffer;
		Nested arrayName;
		
	public:
		MyArray(size_t length, const Nested& n):mLength{length}, mBuffer{new T[length]}, arrayName{n}
		{
			cout<<"My array c-tor"<<endl;
		}

		~MyArray(){delete mBuffer;};
		
		//implement copy semantics
		MyArray(MyArray &obj):mLength(obj.mLength) //,arrayName{ obj.arrayName}
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
			for(size_t i{0}; i < mLength; i++)
				mBuffer[i] = obj.mBuffer[i];

			cout<<"Nested assignment operator"<<endl;
				
			return *this;
		}
		
		//implement move semantics
		MyArray(MyArray &&obj):mLength(obj.mLength), arrayName{move(obj.arrayName)}
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
			//reset obj.mBuffer to point to null =>leave both this and obj in a well defined state
			obj.mLength = 0;
			obj.mBuffer = nullptr;
				
			cout<<"MyArray move assignment operator"<<endl;

			return *this;
		}
		
		size_t getLength(){return mLength;};
		void setlength(size_t length){mLength = length;};
		T& operator[](size_t index){return mBuffer[index];};
		Nested getArrayname() const {return arrayName;};
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
	Nested arrayName{"first array"};
	MyArray<int> arr(3, arrayName);
	arr[0] = 0;
	arr[1] = 1;
	arr[2] = 2;
	
	cout<<"create copy_arr using move c-tor."<<endl;
	MyArray<int> copy_arr(move(arr));
	cout<<" arr length: "<<arr.getLength()<<" copy_arr length: "<<copy_arr.getLength()<<endl;
	for(size_t x{0}; x<copy_arr.getLength(); x++)
		cout<<copy_arr[x]<<" ";
	cout<<endl;
	
	cout<<"recreate arr using move assignment. arr length: "<<arr.getLength()<<" copy_arr length: "<<copy_arr.getLength()<<endl;
	arr = std::move(copy_arr);
	for(size_t x{0}; x<arr.getLength(); x++)
		cout<<arr[x]<<" ";
	cout<<endl;
	
	//use copy semantics
	cout<<"recreate copy_arr using copy assignment. arr length: "<<arr.getLength()<<" copy_arr length: "<<copy_arr.getLength()<<endl;
	copy_arr = arr;
	for(size_t x{0}; x<copy_arr.getLength(); ++x)
		cout<<copy_arr[x]<<" ";
	cout<<endl;

	MyArray<int>&& rref_arr{move(arr)};
	cout<<"create ref to r-value arr using move arr length: "<<arr.getLength()<<" copy_arr length: "<<rref_arr.getLength()<<endl;

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
