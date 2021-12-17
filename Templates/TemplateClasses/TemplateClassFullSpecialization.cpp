#include "TemplateClass.hpp"

//no template delcaration is further required, it is only needed when the class is declared.
void CheckType<bool>::printType() const
{
    std::cout<<"type is bool"<<std::endl;
}