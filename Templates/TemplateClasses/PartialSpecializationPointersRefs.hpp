#pragma once

template<class T>
struct CheckType
{
    static bool IsPlainData(T t){return true;};
    static bool IsRawPointer(T t){return false;};
    static bool IsRef(T t){return false;};
};

template<class T>
struct CheckType<T*>
{
    static bool IsPlainData(T* t){return false;};
    static bool IsRawPointer(T* t){return true;};
    static bool IsRef(T* t){return false;};
};

template<class T>
struct CheckType<T&>
{
    static bool IsPlainData(T& t){return false;};
    static bool IsRawPointer(T& t){return false;};
    static bool IsRef(T& t){return true;};
};
