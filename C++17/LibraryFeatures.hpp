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
*/
