#pragma once

#include <iostream>

inline const int inlineConstInt{2};
//inline acts as an extern, allowing for the same global data to be accessible to multiple .cpp files, without redefining it
inline double inlineDouble{};

//non inline function, defined only once in a .cpp file
void InlineVarFunc();
//inline function, defined in this header file, that is included in multiple .cpp files. 
//Inline allows for having the same function definition multiple times in source code files
inline void InlineFunc()
{
    inlineDouble = 23;
    std::cout<<__func__<<" "<<inlineConstInt<<" "<<inlineDouble<<std::endl;
}
