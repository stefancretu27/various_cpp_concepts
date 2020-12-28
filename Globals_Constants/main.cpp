#include <iostream>

#include "globals_constants.hpp"

using namespace std;
extern void func(const int&);

int main()
{
    cout<<globals_constants::d<<endl;
    func(globals_constants::i);
    return 0;
}