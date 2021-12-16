#include "TemplateMethodsImpl.cpp"

//explicit instantiation of template method with the required types
template void NonTemplateClass::doWork<double>(const double);
template void NonTemplateClass::doWork<bool>(const bool);
template void NonTemplateClass::doWork<long>(const long);

//explicit instantaion of template method that wraps a call of a static template method for inner template struct
template void NonTemplateClass::doWork<double, float>(const double&, const float&);
template void NonTemplateClass::doWork<bool, bool>(const bool&, const bool&);
template void NonTemplateClass::doWork<double, bool>(const double&, const bool&);
template void NonTemplateClass::doWork<bool, float>(const bool&, const float&);
//explicit instantiation of wrapper struct for template method is not required, thus 
//template void NonTemplateClass::EncapsulateTemplateMethod<double, float>::wrappedDoWork(const double&, const float&);
