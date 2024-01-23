
#include <string>
#include <iostream>
#include <bitset>
#include <exception>
#include <vector>
#include <future>
#include <thread>
#include <climits>
/*
* Defined in header <exception>, implementing the std::exception class, from which all standard exceptions derive.
* The interface handles erros thrown via the throw expression.
* The std::exception is used to create exception-type objects and  defines 4 members:
*   - c-tor
*   - virtual d-tor
*   - copy c-tor: used to copy the exception object
*   - virtual what: retruns a string with an explanation associated with the exception
*
* Besides those 4 members, a standard exception must define copy assignment operator. The c-tor in standard exception
* can take a stringa rgument, containing the explanation returned by what()
* 
* Exceptions are thrown by library functions or specifically, using std::throw.
*   Eg: throw std::out_of_range("The array bounds were exceeded");
*
* Exceptions are treated in try-catch blocks. A try block can handle only one thrown exception. There can be multiple catch clauses,
* that should be specified from the most specialized one, to the most generic one.
*/

using namespace std;

void ComputeNumbersInRange0To100(int a, int b)
{
    if(a <0 || b < 0 || a >100 || b > 100)
        throw std::domain_error("Error ar line " + to_string(__LINE__) + " as arguments must be in range 0 to 100 in function " + string(__FUNCTION__));
}

int main()
{
    /*
    * Standard exceptions categories defined in <stdexcept> header:
    *
    * 1.std::logic error. Defines a type of object to be thrown as exception. It reports errors that are a consequence of faulty logic 
    * within the program such as violating logical preconditions or class invariants and may be preventable. It has 5 subcategories:
    *
    *       1.1 std::invalid_argument: It reports errors that arise because an argument value has not been accepted. This exception is thrown by 
    *           std::bitset::bitset, and the std::stoi and std::stof families of functions. 
    *       1.2 std::domain_error: It may be used by the implementation to report domain errors, that is, situations where the inputs are outside 
    *           of the domain on which an operation is defined. The standard library components do not throw this exception. Third-party libraries, however, use this
    *       1.3 std::length_error:  It reports errors that result from attempts to exceed implementation defined length limits for some object.
    *           This exception is thrown by member functions of std::basic_string and std::vector, when create a string/vector bigger than std::string::max_size().
    *       1.4 std::out_of_range: t reports errors that are consequence of attempt to access elements out of defined range.It may be thrown by the member 
    *           functions of std::bitset and std::basic_string, by std::stoi and std::stod families of functions, and by the bounds-checked member access functions 
    *           (e.g. std::vector::at and std::map::at)
    *       1.5 std::future_error: unlinke the above 4, it is defined in <future> header and represents an exception object that is thrown on failure by the 
    *           functions in the thread library that deal with asynchronous execution and shared states (std::future, std::promise, etc). 
    *           Similar to std::system_error, this exception carries an error code compatible with std::error_code. 
    */
    
    try
    {
        std::bitset<4> mask{1234};      //unsigned long long input. Even tho the value cannot be fit on 4 bits, the argument is valid, the output is erroneous
        std::bitset<4> mask2{"124"};  //bitstring input
        std::cout<<mask<<std::endl;
    }
    catch(std::invalid_argument const& iaException)
    {
        std::cout<<__LINE__<<" "<<iaException.what()<<endl;
    }
    //good practice to have a generic catch for any other type of exception not specifically handled in catch clauses above
    catch(std::exception const& ex)
    {
        std::cout<<__LINE__<<" "<<ex.what()<<endl;
    }
    
    try
    {
        auto floatFromString = std::stof("abcdef");
    }
    catch(std::invalid_argument const& iaException)
    {
        std::cout<<__LINE__<<" "<<iaException.what()<<endl;
    }

    
    try
    {
        ComputeNumbersInRange0To100(-1, 34);
        
        vector<char16_t> vecChar;
        vecChar.resize(vecChar.max_size()+1);
    }
    catch(std::domain_error const& deException)
    {
        std::cout<<__LINE__<<" "<<deException.what()<<endl;
    }

    
    try
    {
        vector<char16_t> vecChar;
        vecChar.resize(vecChar.max_size()+1);
    }
    catch(std::length_error const& leException)
    {
        std::cout<<__LINE__<<" "<<leException.what()<<endl;
    }

    
    try
    {
        u16string string16bitChar{u"16-bit char string"};
        cout<<string16bitChar.size()<<" "<<string16bitChar.max_size()<<" "<<string16bitChar.capacity()<<endl;
        cout<<string16bitChar.at(20)<<endl; //out of range

    }
    catch(std::out_of_range const& oorException)
    {
        std::cout<<__LINE__<<" "<<oorException.what()<<endl;
    }

    
    try
    {
        promise<int> promiseInt{};
        future<int> futurePromiseInt; // = promiseInt.get_future();
        cout<<"Future has no associated state as it is not initialized from promise "<<futurePromiseInt.get()<<endl;
    }
    catch(std::future_error const& feException)
    {
        std::cout<<__LINE__<<" "<<feException.what()<<endl;
    }

    
    /*
    * 2.std::runtime_error: It reports errors that are due to events beyond the scope of the program and can not be easily predicted.
    *   2.1 range_error: It can be used to report range errors (that is, situations where a result of a computation cannot be represented by the destination type).
    *                    The only standard library components that throw this exception are std::wstring_convert::from_bytes and std::wstring_convert::to_bytes.
    *                    The mathematical functions in the standard library components do not throw this exception
    *   2.2 overflow_error: It can be used to report arithmetic overflow errors = situations where a result of a computation is too large for the destination type.
    *                       The only standard library components that throw this exception are std::bitset::to_ulong and std::bitset::to_ullong. 
    *   2.3 underflow_error: It may be used to report arithmetic underflow errors = situations where the result of a computation is a subnormal floating-point value
    */
    
    try
    {
        auto lambda = [](auto a, auto b)
                        {
                            if(a < 0 || b < 0)
                            {
                                throw std::invalid_argument("No negative integers are accepted");
                            }
                            if(a + b > USHRT_MAX)
                            {
                                throw std::range_error("Result is greater than the range of values storable on unsigned short");
                            }
                                
                            unsigned short result = a + b;
                        };
                      
        lambda(1234567890, 1234567890);  
        lambda(-2, 3);  //invalid_argument exception not thrown, as the range_error exception is thrown before
    }
    catch(std::range_error const & reException)
    {
        cout<<__LINE__<<" "<<reException.what()<<endl;
    }
    catch(std::invalid_argument const & iaException)
    {
        cout<<__LINE__<<" "<<iaException.what()<<endl;
    }
    
    try
    {
        bitset<128> b(9999);
        unsigned long result = b.to_ulong(); 
        b.flip();
        result = b.to_ulong();
    }
    catch(std::overflow_error const & oeException)
    {
        cout<<__LINE__<<" "<<oeException.what()<<endl;
    }
    
    try
    {
        unsigned int ui{5};
        
        int res = ui-6;
        unsigned short res2 = ui-6;
        cout<<res<<" "<<res2<<endl;
        
        if((int)ui-6 < 0)
            throw underflow_error("underflow");
    }
    catch(std::underflow_error const & ueException)
    {
        cout<<__LINE__<<" "<<ueException.what()<<endl;
    }

    return 0;
}
