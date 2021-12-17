#include "TemplateClass.cpp"

//full specializations do not require instantiation as the types are knwon
//template class CheckType<bool>;
template class CheckType<int>;
template class CheckType<int&>;
template class CheckType<int*>;