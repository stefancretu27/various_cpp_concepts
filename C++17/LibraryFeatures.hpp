#pragma once

#include <any>
#include <charconv>
#include <algorithm>
#include <execution>
#include <filesystem>

/*
* C++17 library features
*   1. <any> part of the general library, defines 2 new classes:
*       1.1. std::any: class which implements a type-safe container for single values of any copy constructible type. 
*               Methods: c-tor, complete copy and move semantics, d-tor, 
*                       template<class T, class ... Args> emplace(args...) - calls c-tor of type T with the args provided to emplace, 
*                       reset(), has_value()
*                       type() - returns typeid of the contained object. To be printed bay calling name()
*       1.2. std::bad_any_cast: an exception class type thrown when std::any_cast fails. For example, when casting to other type than the contained object's type.
*
*    Also, introduces 2 new functions:
*       1.3. template<class T, class ... Args>
*            std::make_any: Constructs a std::any object containing an object of type T, passing the provided arguments to T's constructor. 
*       1.4.template<class T> 
*           std::any_cast<>: provides type safe access to the contained object in an std::any instance
*
*   2. <charconv> part of the string library, defines 3 new classes:
*       2.1 chars_format: enum class specifying the formating for floating point when used by to_chars or from_chars
*               enum class chars_format {
*                   scientific = //unspecified,
*                   fixed = //unspecified,
*                   hex = //unspecified,
*                   general = fixed | scientific};
*       2.2 from_chars_result: it is a struct with 2 members (ptr and ec), being the return type of std::from_chars
*       2.3 to_chars_result: it is a struct with 2 members (ptr and ec), being the return type of std::to_chars
*            - ec equals value-initialized std::errc 
*            - ptr is the one-past-the-end pointer of the characters written
*
*    Also, defines 2 new functions, more lightweight parsers than stringstream:
*       2.4. from_chars(const char* first, const char* last, &value, int base): converts a character sequence to an integer 
*                      (const char* first, const char* last, &value, std::chars_format fmt = std::chars_format::general): convert to floating point 
*       2.5. to_chars(char* first, char* last, value, int base = 10 ): value is converted to a string of digits in the given base 
*                    (char* first, char* last, value, float)
*               first, last - character range to write to 
*               value - the value to convert to its string representation
*       On error, std::errc::value_too_large is stored in ec, a copy of the value last in ptr, and leaves the contents of the range [first, last) in unspecified state. 
*       
*       std::to_chars and std:;from_chars are designed to have as little footprint as possible. You provide the buffer, and they do very little beyond actually 
*       formatting the numeric value into/from the buffer, in a specific format, without any locale-specific considerations (as stringstream does), with the only 
*       overhead of making sure that the buffer is big enough. Code that uses std::to_chars or std::from_chars does not need to do any dynamic allocation, unlike
*       to_string which allocates some memory (not necessarily dinamically) for the newly built string.
*
*   3.<execution> is part of the algorithms library, introduces the namespace std::execution, and introduces 5 classes:
*       3.1. is_execution_policy - template class that checks whether T is a standard or implementation-defined execution policy type, storing the boolean result in
*           its value member.
*                   is_execution_policy_v = std::is_execution_policy<T>::value
*       3.2. class sequenced_policy - instantiated as seq, require that a parallel algorithm's execution may not be parallelized
*       3.3. class parallel_policy - par, indicate that a parallel algorithm's execution may be parallelized, either in the calling thread or a newly created one, by
*           the algorithm's implementation
*       3.4. class parallel_unsequenced_policy - par_unseq, indicate that a parallel algorithm's execution may be parallelized, vectorized, or migrated across threads.
*           The elements access is unsequenced with respect to each other, and may execute in unspecified threads (ie. when calling predicate p on each element).
*       3.5. class unsequenced_policy - unseq, indicate that a parallel algorithm's execution may be vectorized, e.g., executed on a single thread using instructions 
*           that operate on multiple data items.
*
*       These instances are used to specify the execution policy of parallel algorithms, i.e., the kinds of parallelism allowed. Additional execution policies 
*       may be provided by a standard library implementation (possible future additions may include std::parallel::cuda and std::parallel::opencl.
*       Unsequenced execution policies are the only case where function calls are unsequenced with respect to each other, meaning they can be interleaved. This implies
*       they cannot use synchronization mechanisms or perform memory allocations, when accessing the input elements, namely in predicate p.
*
*       Hence, since c++17 all algorithm in std::algorithm are overloaded such that their first argument is an execution policy type.
*           eg: template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate >
*               bool all_of( ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate p )
*               
*               template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate >
*               ForwardIt find_if_not( ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate q )
*
*   4. <filesystem>: introduces the std::filesystem namespace, encapsulating the following classes:
*       4.1. path - Objects of type path represent paths on a filesystem. The pathname may represent a non-existing path or even one that is not allowed to exist on
*                   the current file system or OS
*                 - c-tor with const char* and string args, copy and move semantics 
*                 - assign - assign the input pathname character range to use, represented as std::string/std::string_view/pointer to a null-terminated string/
*                           or as an input iterator with char value type that points to a null-terminated multibyte string  
*                 - apend and operator/= - append the input path the the path instance
*                 - concat and operator+= - Unlike with append() or operator/=, additional directory separators are never introduced. 
*                 - native/c_str
*                 - compare - lexicographically compare 2 paths
*                 - empty(): checks if file path is empty
*                Decomposition methods, each one has a corresponding has_"part"() counterpart for checking the existence of that file part
*                 - root_name(): Returns the root name of the generic-format path. If the path (in generic format) does not include root name, returns path().
*                 - root_path(): Returns the root path of the path. If the path does not include root path, returns path(). Returns root_name() / root_directory().
*                 - root_directory(): Returns the root directory of the generic-format path. If the path does not include root directory, returns path(). 
*                 - relative_path(): returns path relative to the root path  
*                 - parent_path(): returns the path of the parent path
*                 - filename(): returns the filename path component  
*                 - stem(): returns the stem path component (filename without the final extension)
*                 - extension(): returns the file extension path component 
*
*     4.2. filesystem_error: defines an exception object that is thrown on failure by the throwing overloads of the functions in the filesystem library. 
*                           - path1(), path2() - return paths involved in operation
*
*     4.3. directory_entry: Represents a directory entry. The object stores a path as a member and may also store additional file attributes (hard link count, 
*                           status, symlink status, file size, and last write time) during directory iteration.
*                          - exists(): checks if path the underlying path refers to an existing path
*                          - is_regular_file()/is_directory()/is_other(): checks if path the underlying path refers to a regular file/directory path/other file 
*                          - is_fifo()/is_socket(): checks if path the underlying path refers to a named pipe/named IPC socket
*
*     4.4. directory_iterator: iterates over the directory_entry elements of a directory (but does not visit the subdirectories). The iteration order is 
*                              unspecified, except that each directory entry is visited only once. Initialized via c-tor from a filesystem::path object
*
*     4.5. recursive_directory_iterator: iterates over the directory_entry elements of a directory and, recursively, over the entries of all subdirectories. The iteration order is 
*                              unspecified, except that each directory entry is visited only once. Initialized via c-tor from a filesystem::path object
*
*     4.6 file_status: Stores information about the type and permissions of a file. 
*                      - std::filesystem::perms/void permissions(void/std::filesystem::perms): returns/sets permissions
*                      - void/std::filesystem::file_type type(std::filesystem::file_type type/ void): sets/returns file type
*
*    4.7 struct space_info {
*              std::uintmax_t capacity;
*              std::uintmax_t free;
*              std::uintmax_t available;};
*      Represents the filesystem information as determined by filesystem::space.
*
*     Also, there are several function helpers impleme4nted in the header:
*      - bool create_directory(const std::filesystem::path& p)/bool create_directories(const std::filesystem::path& p)
*      - bool create_hard_link(const std::filesystem::path& target, const std::filesystem::path& path)/bool create_symlink(target, path)/bool create_directory_symlink(target, path)
*      - path current_path(): returns or sets current path to the eventual path arg
*      - bool exists(path): checks is path exists
*      - void copy(path from, path to)/void rename(path old, path new): copy/rename file or directory
*      - void copy_file(path from, path to): copy file contents
*      - size_t file_size()/void resize_file(path file, size_t new_size): retruns/sets file size
*      - std::filesystem::space_info space(path p): determines available free space on the file system 
*      - std:;filesystem::status(path p): gets file_status
*
*   5. <memory_resources> introduces the namespace std::pmr, the following classes
*       5.1 polymorphic_allocator: is a template class that implements a different allocation behavior depending on the type it is constructed from,
*           that is a pmr::memory_resource which uses runtime polymorphism for managing allocations. It receives as ctyor argument pointer to memory_resource.
*       5.2 memory_resource: The class std::pmr::memory_resource is an abstract interface to an unbounded set of classes encapsulating memory resources. 
*           Methods: - void* allocate( std::size_t bytes): allocates memory of indicated size_t
*                    - void deallocate(void* p, std::size_t bytes): deallocates memory pointed by p, that was returned by allocate
*       5.3 pool_options: is a set of constructor options for pool resources including synchronized_pool_resource and unsynchronized_pool_resource. 
*           Members: - std::size_t max_blocks_per_chunk = max # of blocks that will be allocated at once from the upstream std::pmr::memory_resource to fill the pool.
*                    - std::size_t largest_required_pool_block = largest allocation size that is required to be fulfilled using the pooling mechanism
*
*       Types of memory_resource:
*       5.4 synchronized_pool_resource: a thread-safe memory_resource for managing allocations in pools of different block sizes.
*                                       It owns the allocated memory and frees it on destruction, even if deallocate has not been called for some of the allocated 
*                                       blocks. It consists of a collection of pools that serves requests for different block sizes. Each pool manages a collection 
*                                       of chunks that are then divided into blocks of uniform size. 
*       5.5 unsynchronized_pool_resource: same as above only that is thread-unsafe 
*       5.6 monotonic_buffer_resource: is a special-purpose memory resource class that releases the allocated memory only when the resource is destroyed. 
*                                      It is intended for very fast memory allocations in situations where memory is used to build up a few objects and then is 
*                                      released all at once. It is not thread safe.
*
*       STL containers can be instantiated also from pmr namespace (pmr::list, pmr::deque, pmr::vector etc), eventually taking polymorphic_allocator instance as
*       ctor argument.
*
*   6. <optional> intrdouces the following classes:
*       6.1 optional: a class template that manages a value that may or may not be present. There are no optional references; a program is ill-formed if it 
*                     instantiates an optional with a reference type
*                     
*                     A common use case for optional is the return value of a function that may fail. As opposed to other approaches, such as std::pair<T, bool>, 
*                     std::optional handles expensive-to-construct objects well and is more readable, as the intent is expressed explicitly. 
*
*                     If an optional<T> contains a value, the value is guaranteed to be allocated as part of the optional object => no dynamic memory allocation
*                     occurs. Thus, an optional object models an object, not a pointer, even though operator*() and operator->() are defined.
*
*       6.2 nullopt_t: is an empty class type used to indicate optional type with uninitialized state.
*                      std::nullopt is a constant of type std::nullopt_t that is used to indicate optional type with uninitialized state. 
*
*   7. <string_view> while std::string offers dynamic memory management and mutable operations, std::string_view provides a lightweight, 
*                    non-owning, read-only view of a string, which allows for search/find/access operations. Some of the encapsulated methods are
*                        - char [], char at(index), char front(), char back(), const char* data() 
*                        - copy(*dest, count, pos = 0), bool empty(), size(), length(), max_size()
*                        - substr(pos = 0, count = npos), int compare - compare 2 string_views
*                        - find(view v,  pos = 0) - finds the first substring equal to the given character sequence. 
*                        - rfind(view v,  pos = 0) - finds the last substring equal to the given character sequence. 
*
*       7.1 basic_string_view: template class implementing read-only string view, describing an object that can refer to a constant contiguous sequence CharT, with the 
*                               first element of the sequence at position zero. A typical implementation holds only two members: a pointer to constant CharT and a size.
*       7.2 std::string_view: std::basic_string_view<char>
*       7.3 std::u16string_view:  std::basic_string_view<char16_t>
*       7.4 std::u32string_view:  std::basic_string_view<char32_t>
*       7.5 std::wstring_view:  std::basic_string_view<wchar_t>
*
*   8. <variant> defines the following classes:
*       8.1 variant: represents a type-safe union. An instance of std::variant at any given time either holds a value of one of its alternative types, or in the 
*                   case of error - no value.
*                   As with unions, if a variant holds a value of some object type T, the object representation of T is allocated directly within the object 
*                   representation of the variant itself. Variant is not allowed to allocate additional (dynamic) memory. 
*
*                   A variant is not permitted to hold references, arrays, or the type void. A variant is permitted to hold the same type more than once, 
*                   and to hold differently cv-qualified versions of the same type. 
*
*                   A program that instantiates the definition of std::variant with no template arguments is ill-formed. 
*
*                   Methods: variadic template c-tor, copy and move semantics; size_t index() - returns the 0-based position of the currently active data type;
*                            bool valueless_by_exception() - checks if the variant is in the invalid state
*                           template<class Type, ... Args> emplace(...args): creates a new value in-place, in an existing variant object, using as template arg
*                           the data type or the index of the field to be activated and set
*
*       8.2 visit: template function having the signature below, applies the visitor vis (a Callable that can be called with any combination of types from variants) 
*                  to the variants vars 
*                            template< class Visitor, class... Variants >
*                            constexpr  visit( Visitor&& vis, Variants&&... vars )
*
*   9. std::invoke: defined in header <functional>, it is a template function that wraps a callable object and its args, returning the wrapped's callable value
*                   template< class F, class... Args > 
*                   std::invoke_result_t<F, Args...> invoke( F&& f, Args&&... args )
*
*                   std::bind enables you to create new function objects on the fly, with the returning callable being used as callback argument, whilst std::invoke
*                   calls the callable on the spot.
*
*   10. std::weak_from_this<T>: creates a weak_ptr observer to *this, created when the first shared_ptr<T> is created
*
*   11. std::scoped_lock: defined in <mutex> it is a mutex wrapper that provides a convenient RAII-style mechanism for owning zero or more mutexes for the duration 
*                         of a scoped block. When a scoped_lock object is created, it attempts to take ownership of the mutexes it is given. When control leaves 
*                         the scope in which the scoped_lock object was created, the scoped_lock is destructed and the mutexes are released. If several mutexes are 
*                         given, deadlock avoidance algorithm is used as if by std::lock. The scoped_lock class is non-copyable. 
*
*   12. std::shared_mutex: The shared_mutex class is a synchronization primitive that can be used to protect shared data from being simultaneously accessed by 
*                          multiple threads. In contrast to other mutex types which facilitate exclusive access, a shared_mutex has two levels of access:
*                               - shared - several threads can share ownership of the same mutex. 
*                               - exclusive - only one thread can own the mutex. 
*                         If one thread has acquired the exclusive lock (through lock, try_lock), no other threads can acquire the lock (including the shared).
*                         If one thread has acquired the shared lock (through lock_shared, try_lock_shared), no other thread can acquire the exclusive lock, 
*                         but can acquire the shared lock. Within one thread, only one lock (shared or exclusive) can be acquired at the same time. 
*
*   13. std::shared_ptr<T>::operator[]: Index into the array pointed to by the stored pointer. std::make_shared not supported with arrays.
*                                       Syntax: std::shared_ptr<T[]> sp {new T[dim]};
*                                               sp = std::shared_ptr<T[]>{new T[dim]};
*
*   14. std::tuple: - deduction guides. Since the size, the types of its elements, and the ordering of those types are part of its type signature, 
*                     they must all be available at compile time and can only depend on other compile-time information. Thus, upon instantiating a tuple,
*                     the template types are deduced form arguments passed to tuple c-tor. When returning a tuple from function, the template types are specified
*                     in the return type, but not necessary anymore in the return statement.
*
*                   - T make_from_tuple(Tupple&): Construct an object of type T, using the elements of the tuple t as the arguments to the constructor => calls
*                                                 T's ctor by untying the tuple's elements such that they match the ctor args.
*
*                   - constexpr decltype(auto) apply( F&& f, Tuple&& t ): invoke the Callable object f with the elements of t as arguments. 
*/

//optional
std::optional<std::unique_ptr<int>> AllocateUPtr(const int value)
{
    std::optional<std::unique_ptr<int>> result{std::nullopt};
    
    result = std::make_unique<int>(value);
    
    return result;
}
 
template<class T, std::enable_if_t<std::is_fundamental<T>::value, bool> = true>
void SetValue(T value)
{
    std::optional<T> optT{};
    
    if constexpr(std::is_integral<T>::value)
    {
        optT = value;
    }
    else if constexpr(std::is_void<T>::value || std::is_null_pointer<T>::value)
    {
        optT = std::nullopt;
    }
    
    if(optT)
    {
        std::cout<<*optT<<std::endl;
    }
}

//visit and Variant
//struct that encapsulates multiple overloads of the operator()
template<class ...CallableTypes>
struct CallableWrapper : public CallableTypes...
{
    using CallableTypes::operator()...;
};

//user guided decution for the above
template<class ...CallableTypes>
CallableWrapper(CallableTypes...) -> CallableWrapper<CallableTypes...>;

//weak_from_this() method from enable_shared_from_this<T> class
class MyClass : public std::enable_shared_from_this<MyClass>
{
    public:
    void ShMethod()
    {
        if(std::shared_ptr<MyClass> shPtr = shared_from_this(); shPtr)
        {
            shPtr->i = 2;
            shPtr->d = 2.7182;
            std::cout<<shPtr.use_count()<<" ";
        }
        std::cout<<__func__<<" "<<weak_from_this().use_count()<<std::endl;
    }
    
    void WkMethod()
    {
        if(std::shared_ptr<MyClass> shPtr = weak_from_this().lock(); shPtr)
        {
            shPtr->i = 2;
            shPtr->d = 2.7182;
            std::cout<<shPtr.use_count()<<" ";
        }
        std::cout<<__func__<<" "<<weak_from_this().use_count()<<std::endl;
    }
    
    private:
    int i{};
    double d{};
};

//tuple
std::tuple<int, double, char> GetTuple()
{
    return {5, -3.09, 'g'};
}

struct Example
{
    Example(int i, double d, char c):mi{i}, md{d}, mc{c}
    {};
    
    int mi;
    double md; 
    char mc;
};
