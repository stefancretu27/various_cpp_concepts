#pragma once


#include <iostream>
#include <cstring>

struct Shallow 
{
    char* data;

    Shallow(const char* str) 
   {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    // Implicit copy constructor: shallow copy
    // Implicit move constructor: shallow move, just moves the pointer, by copying the pointer address, but not transferring the ownership of the container => 
    // both pointers point to the same memory, leading to double free when d-tor is called for the 2nd (moved to) instance

    ~Shallow() 
   {
        delete[] data;
    }
};

struct Deep 
{
    char* data;

    Deep(const char* str) 
    {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    // Deep copy
    Deep(const Deep& other) 
    {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
    }

    // Deep move
    Deep(Deep&& other) noexcept 
    {
        data = other.data;
        other.data = nullptr;
    }

    ~Deep() 
    {
        delete[] data;
    }
};
