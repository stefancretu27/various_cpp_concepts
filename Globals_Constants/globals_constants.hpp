#ifndef GLOBALS_CONSTANTS
#define GLOBALS_CONSTANTS

#include <string>

/*
 * External linkage = multiple declarations refer to the same entity (variable/function) within the entire project's files
 *  - global variables/functions
 *  - extern const global variables
 *  - user defined types declared in global scope (struct/class/enum/enum class)
 * Internal linkage = the identifier's' declaration refers to all its occurences within the same file, but not in other files
 *  - static variables
 *  - static functions
 *  - const globals
 * No linkage:
 *  - local variables
 */ 

namespace globals_constants
{
	//declare global const in hpp => it has internal linkage and each file gets its own definition
    const double d{3.14151};
    
    //In order to get one copy of the global constant in the entire project: forward declare using extern to change their linkage to external. 
    //Thus, their definitions shall occur in another file.
    extern const char c;
    extern const std::string name;
    
    //as constexpr must be initialized upon declaration, they cannot be forward declared, thus they are defined directly in hpp file, 
    //which means each cpp will get its definition =>changing it requires recompiling each cpp that uses it
    constexpr int i{8};
    
    //declare global non-const => it has external linkage and it will cause multiple definitions error if the header is included in more source files
    //float f; =>error multiple definitions 
    //solution is to define it in cpp file and forward declare it in hpp file
    extern float f;
}

#endif
