#include "TemplateMethods.hpp"

//this source file implements all non template methods
NonTemplateClass::NonTemplateClass(uint_least16_t u):mUInt16{u}
{
    std::cout<<"NonTemplate c-tor"<<std::endl;
}

void NonTemplateClass::doWork(const bool b)
{
    std::cout<<b<<std::endl;
}