#ifndef MOVECLASS_HPP
#define MOVECLASS_HPP

#include <iostream>
#include <vector>

using namespace std;

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

#endif