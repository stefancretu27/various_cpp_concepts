#ifndef GLOBALS_CONSTANTS
#define GLOBALS_CONSTANTS

#include <string>

namespace globals_constants
{
    //forward declare but not define globals and constants, by using extern to specify that their definitions shall occur in another file
    extern const char c;
    extern const std::string name;
    //as constexpr must be initialized upon declaration, they cannot be forward declared, thus they are defined directly in hpp file, 
    //which means each cpp will get its definition =>changing it requires recompiling each cpp that uses it
    constexpr int i{8};
    //declare global const in hpp => it has internal linkage and each file gets its own definition
    const double d{3.14151};
    //declare global non-const => it has external linkage and it will cause multiple definition error
    //float f; =>error multiple definition
    //solution is to define it in cpp file and forward declare it in hpp file
    extern float f;
}

#endif