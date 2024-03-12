#include "InlineVariable.hpp"

void InlineVarFunc()
{
    inlineDouble = 8;
    std::cout<<__func__<<" "<<inlineConstInt<<" "<<inlineDouble<<std::endl;
    
    InlineFunc();
}
