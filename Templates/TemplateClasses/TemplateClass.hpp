#pragma once

#include <iostream>

template<class T>
class CheckType
{
    public:
    void printType() const;
};

//full specialization for bool
template<>
class CheckType<bool>
{
    public:
    void printType() const;
};

//partial specialization for pointers
template <class T>
class CheckType<T*>
{
    public:
    void printType() const;
};

//partial specialization for references
template <class T>
class CheckType<T&>
{
    public:
    void printType() const;
};

