//for size_t
#include <cstdio>
//for move
#include <utility>
#include <iostream>

using namespace std;

template<typename T>
class MyArray
{
	private:
		size_t mLength;
		T* mBuffer;
		
	public:
		MyArray(size_t length):mLength(length),mBuffer(new T[length])
		{
			
		}
		~MyArray(){delete mBuffer;};
		
		//disable copy semantics
		MyArray(MyArray &obj):mLength(obj.mLength)
		{
			mBuffer = new T[mLength];
			
			//deep copy
			for(int i = 0; i < mLength; i++)
				mBuffer[i] = obj.mBuffer[i];
			
		};
		MyArray& operator=(MyArray &obj)
		{
			if(this==&obj)
				return *this;
			
			delete [] mBuffer;
			
			mLength = obj.mLength;
			mBuffer = new T[mLength];
			
			//deep copy
			for(int i = 0; i < mLength; i++)
				mBuffer[i] = obj.mBuffer[i];
				
			return *this;
		}
		
		//implement move semantics
		MyArray(MyArray &&obj):mLength(obj.mLength)
		{
			//do not allocate separate memory and copy data as in copy c-tor, but set the this->mBuffer pointer to point to the same memory location as obj.mBuffer
			mBuffer = obj.mBuffer;
			//now the buffer is pointed to only by this->mBuffer, as obj.mBuffer will point to null
			obj.mBuffer = nullptr;
			obj.mLength = 0;
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
				
			return *this;
		}
		
		int getLength(){return mLength;};
		void setlength(size_t length){mLength = length;};
		T& operator[](size_t index){return mBuffer[index];};
};

int main()
{
	MyArray<int> arr(3);
	arr[0] = 0;
	arr[1] = 1;
	arr[2] = 2;
	
	MyArray<int> copy_arr(move(arr));
	
	cout<<"create copy_arr using move c-tor. arr length: "<<arr.getLength()<<" copy_arr length: "<<copy_arr.getLength()<<endl;
	
	for(int x{0}; x<copy_arr.getLength(); x++)
		cout<<copy_arr[x]<<" ";
	cout<<endl;
	
	arr = std::move(copy_arr);
	
	cout<<"recreate arr using copy assignment. arr length: "<<arr.getLength()<<" copy_arr length: "<<copy_arr.getLength()<<endl;
	
	for(int x{0}; x<arr.getLength(); x++)
		cout<<arr[x]<<" ";
	cout<<endl;
	
	//use copy semantics
	copy_arr = arr;
	cout<<"recreate copy_arr using copy semantics. arr length: "<<arr.getLength()<<" copy_arr length: "<<copy_arr.getLength()<<endl;
	for(int x{0}; x<copy_arr.getLength(); x++)
		cout<<copy_arr[x]<<" ";
	cout<<endl;
	
	
	return 0;
}
