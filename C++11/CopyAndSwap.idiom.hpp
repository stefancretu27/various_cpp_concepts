#include <list>
#include <iostream>
#include <initializer_list>
		
class Composed
{
    public:
    Composed() = default;
    
    explicit Composed(const std::initializer_list<int>& initList)
    {
        mElements.assign(initList);
        size = initList.size();
        container = new int[size];
        std::copy(initList.begin(), initList.end(), container);
        std::cout<<"C-tor initializer_list"<<std::endl;
    };
    
    Composed(const std::list<int>& list):mElements{list}
    {
        size = list.size();
        container = new int[size];
        std::copy(list.begin(), list.end(), container);
        std::cout<<"C-tor list"<<std::endl;
    };
    
    Composed(const Composed& rhs):mElements{rhs.mElements}, size{rhs.size}
    {
        container = new int[size];
        std::copy(rhs.mElements.begin(), rhs.mElements.end(), container);
        std::cout<<"Copy c-tor"<<std::endl;
    };
    
    friend void swap(Composed& lhs, Composed& rhs)
    {
        std::cout<<__FUNCTION__<<std::endl;
        using std::swap;
        swap(lhs.mElements, rhs.mElements);
        swap(lhs.size, rhs.size);
        swap(lhs.container, rhs.container);
    }
    
    //Default construct the object to move to, then swap it with rhs.The goal of move-construction: to take the resources from another 
    //instance of the class, leaving it in a state guaranteed to be assignable and destructible (well defined state).
    Composed(Composed&& rhs):Composed()
    {
        std::cout<<"Move c-tor"<<std::endl;
        swap(*this, rhs);
    };
    
    //Copy-and-swap idiom: creating a single one implementation for (both copy and move) assignment operator, that is exception safe and has no code duplicates.
    //It requires copy c-tor to create a copy of the rhs as argument, leveraging copy ellision, and d-tor to destruct this copy at the end. 
    //If rhs is a r-value, then the move c-tor is needed. It also requires a friend swap function, which knows how to swap individual members.
    //Then the contents of the existing *this object are swapped with the copy's.
    Composed& operator=(Composed rhs)
    {
        std::cout<<"Assignment"<<std::endl;
        swap(*this, rhs);
        return *this;
    }
    
    void PrintElements()
    {
        std::cout<<__FUNCTION__<<std::endl;
        for(const int& elem : mElements)
        {
            std::cout<<elem<<" ";
        }
        std::cout<<std::endl;
    }
    
    void PrintContainer()
    {
        std::cout<<__FUNCTION__<<std::endl;
        for(size_t idx{0}; idx < size ;++idx)
        {
            std::cout<<container[idx]<<" ";
        }
        std::cout<<std::endl;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const Composed& inst)
    {
        out<<__func__<<std::endl;
        for(const int& elem : inst.mElements)
        {
            out<<elem<<" ";
        }
        out<<std::endl;
        
        return out;
    }
    
    virtual ~Composed()
    {
        delete [] container;
        container = nullptr;
        size = 0;
    }
    
    private:
    std::list<int> mElements{};
    int* container{nullptr};
    size_t size{};
};


		
