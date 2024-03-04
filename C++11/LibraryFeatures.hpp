#pragma once

#include <list>
#include <forward_list>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <array>
#include <vector>
#include <deque>
#include <type_traits>
#include <functional>
#include <numeric>
#include <random>

/*
* 1. Concurrency support libraries:
*   1.1 <thread>: thread class (join, detach, swap, joianble, get_id) creating objects that allow the program to execute multiple tasks concurently. It is the smallest
*               entity that can be managed by a scheduler. It's c-tor takes as parameter a function and its arguments (by copy), if any, without returning any value 
*               + this_thread namespace: yield (reschedule thread execution), sleep_for, sleep_until, get_id
*
*   1.2 <atomic>: wrapps scalar, PODs, trivial data types to allow for thread-safe operations, without using any synchronization mechanisms => 
*               read-modify-write operations take place in a single total order, perceived as a singular, indivisible operation: load, store, exchange, fetch_add, 
*               fetch_or, fetch_sub, fetch_and
*
*   1.3 <mutex>: synchronization mechanisms that block all threads but 1 that attempt to access a critical section/shared resource.
*               - Mutex types: mutex, timed_mutex, recursive_mutex (used by the same thread, when resource is accessed recursively), recursive_timed_mutex.
*                Methods: lock(), unlock(), try_lock() - returns bool; for timed mutexes: try_lock_for(), try_lock_until()
*               - Mutex wrappers, allow for owning mutexes in RAII style: lock_guard, unique_lock (offers interface for lock, unlock, try_lock operations => used with 
*               condition_variable), defer_lock (used with unique_lock, for more mutexes locked simultaneously), adopt_lock (used with lock_guard for same thing)
*
*   1.4 <condition_variable>: synch mechanism that blocks all threads waiting for being notified upon status change on the shared resource. Notification and
*               waiting operations are done via the condition_variable. Spurious wake-ups occurs when more threads are notified, one wakes-up, but another one
*               proceeds in accessing the shared resoruce => extra predicate function is used on waiting to mititgate this
*               - condition_variable takes as 1st argument only unique_lock, whereas condition_variable_any works with any mutex type
*               Methods: notify_one, notify_all, wait, wait_for, wait_until
*
*   1.5 <future>: - {promise, future}, promise allows a thread to pass a value to another thread, before completion (threads are joined), being the wirting end. 
*               The value is accessed via the future associated with the promise, which is the reading end. Acts more like a unidirectional communication pipe, that
*               has a shared state associated with it. shared_future can be used to get a copy of the unique reading end of type future.
*               Methods: promise {get_future, set_value, set_value_at_thread_exit}, future {wait, wait_for, wait_until, share, valid, get} 
*               - {packaged_task, future}: packaged_task is a class template defining a function wrapper that can be used to execute the function asynchronously, 
*               whilst allowing to return the value via the future associated with the packaged_task instance. The wrapped function is specified exclusively via 
*               list or direct initialization () or {}, not by copy initialization =. The packaged_task can be used as thread execution function
*               - {async, function}: is a function template which can execute the function associated with it asynchronously, maybe in another thread, allowing for
*               returning from it to the caller, via the future returned by async call. launch::async or launch::deferred execution policies. 
*
* 2. emplace: part of STL, emplace(), emplace_back(), emplace_front(), emplace_hint(), methods allowingdirect construction of the object in the given container,
*               as the c-tor arguments are directly passed to the method
*
* 3. <forward_list>: part of the STL, implements a single linked list, enabling a forward_iterator only, which does not allow to iterate from end (no reverse_iterator)
*
* 4. unique_ptr<T>: defined in header memory, a single instance pointing to a dinamically allocated memory
*
* 5. <initializer_list>: lightweight container of const T elements, mainly used to initialize container from list initialization
*
* 6. <chrono>: chrono:: namespace: system_clock (clock starting from OS time), steady_clock (monotonic clock that will never be adjusted ), high_resolution_clock
*               (the shortest tick period available). Methods: now(), return a time_point value. Difference between 2 time_point represents a duration object, that
*               is a time interval. duration_cast is used to convert duration to another representation (hour/minutes/seconds/milliseconds/microseconds/nanoseconds)
*
* 7.<algorithm>:The algorithms library defines functions for a variety of purposes (e.g. searching, sorting, counting, manipulating) that operate on ranges of 
*               elements. Note that a range is defined as [first, last) where last refers to the element past the last element to inspect or modify. 
*
*               Those that require an extra predicate, it should be a function whose parameter(s) have the same type as the container(s)
*    7.1 Search operations: - find, find_if, find_if_not (firstIt, lastIt, val/pred/!pred). Value search/predicate-based search. Return iterator to firstly found occurence.
*                           - any_of, all_off, none_off(firstIt, lastIt, pred). Predicate-based search, return bool.
*                           - count, count_if(firstIt, lastIt, val/pred). Value/predicate-based search, return the number of iterators it in the range [first, last) satisfying the condition
*                           - search, find_end (firstIt, lastIt, s_first, s_last): search for 1st/last occurence of subrange [s_first, s_last) in range [firstIt, lastIt), returns iterator
*
*    7.2 Copy operations: - copy(firstIt, lastIt, output_firstIt): copies elements of input range to the range that starts with output_firstIt
*                         - copy_if(firstIt, lastIt, output_firstIt, pred): copies elements of input range to the range that starts with output_firstIt, if they satisfy condition pred (function object)
*                         - copy_n(firstit, count, output_firstIt): copies first n elements of input range, that sarts at firstIt, to the range that starts with output_firstIt 
*                         - copy_backward(firstIt, lastIt, output_lastIt): copies elements of input range, in reverse order, to the range that ends with output_lastIt
*                         - move(firstIt, lastIt, output_firstIt): moves elements of input range to the range that starts with output_firstIt, 
*                         - move_backward(firstIt, lastIt, output_lastIt):returns iterator to one past the last copied elem. Moves elements of input range, in reverse order, to the range that ends with output_lastIt
*
*    7.3 Swap operations: - swap(var1, var2) => no return value, swaps values of the 2 input variables
*                         - iter_swap (elem_it1, elem_it2) => no return value, swaps values of the 2 input iters
*                         - swap_ranges (first1, last1, first2) swap containers, or subcontainers. Returns iterator to the element past the last element exchanged in the second range. 
*
*    7.4 Transformation operations: - std::transform(firstIt, lastIt, output_firstIt, func_object) applies the given function to a range and stores the result in another range, beginning at d_first
*                                                                                                   keeping the original elements order. Returns it to next to last transformed elem
*                                   - replace/replace_if(firstIt, lastIt, old_val/pred, new_val): replace all elements equal to old value/that satisfy the predicate, with the new value. No return value.
*
*    7.5 Generation operations: - fill/fill_n(firstIt, lastIt/size count, value): assign value to range/first n elems in range, no return
*                               - generate/generate_n(firstIt, lastIt/size count, func_object): assign value generated by input function object to range/first n elems in range, no return 
*
*    7.6 Removing operations: - remove/remove_if(firstIt, lastIt, value/predicate): Removes all elements from the range [firstIt, lastIt) that are equal to input value/satisfy input predicate,
*                                                                                   with the remaining elements being copied to the first positions, return iterator to element past remaining elements range
*                             - remove_copy/remove_copy_if(firstIt, lastIt, outputIt, value/predicate): copies elems fron input range to output range, removing those elems equal to input value/satisfying
*                                                                                                           input predicate p, returns iterator to the element past the last element copied.
*                             - unique/unique_copy(firstIt, lastIt, output_firstIt): eliminates duplicate elements in consecutive sub ranges, shifting the remaining elems to the first positions and 
*                                                                                       returns iterator past last not removed element (past logical end). (1,1,2,3,3,4,5,5) -> (1,2,3,4,5, garbage, garbage)
*
*    7.7 Order changing operations: - reverse/reverse_copy(firstIt, lastIt, output_firstIt): reverses the order of values, eventually copying the result in another container. No return
*                                   - rotate/rotate_copy(firstIt, middleIt, lastIt, output_firstIt): places the elements in range [firstIt, middleIt) after the elements [middleIt, lastIt). For copy,
*                                                                                                    the rotated elements are copied in the container whose begin() is output_firstIt. Returns iterator
*
*    7.8 Partitioning operations: - is_partitioned(firstIt, lastIt, pred): returns bool as it checks if all elements in the range [firstIt, lastIt) that satisfy the predicate pred appear before 
*                                                                           all elements that don't.
*                                 - partition_point(firstIt, lastIt, pred): returns the end of the first partition, that is, the first element that does not satisfy p, or lastIt if all satisfy
*                                 - partition, partition_copy(firstIt, lastIt, pred, out_firstIt, out_secondIt): divides the range in 2 subranges, the first range contains elements that satisfy pred, elems that
*                                                                                                                don't are moved in the 2nd range. Eventually, range are copied them to 2 distinct ranges (true
*                                                                                                                range/false range). Returns iter to 1st elem of the second group/pair of iter to past last elem 
*                                                                                                                in each range
*
*    7.9 Sorting operations: - sort(firstIt, lastIt), sorts elements in range, by default in ascending order, in O(n*logn)=>qsort. Equal elements' order might not be preserved
*                            - partial_sort(firstIt, midIt, lastIt), sorts elements in range [firstIt, midIt), by default in ascending order
*                            - is_sorted(firstIt, lastIt), is_sorted_until(firstIt, lastIt): check if the range/the largest range is sorted in ascending order. Returns bool/iterator
*                            - nth_element(firstIt, nthIt, lastIt): changes the nth element, putting on its position the value that would have been there if the range was sorted in ascending order.
*                                                                   Before its position are elements whose values are less than or equal to those placed after its position
*
*    7.10 Binary search operations: - lower_bound(firstIt, lastIt, val): returns iterator to the first element in input range such that val<=elem (for which val is a lower bound). The range must be partitioned
*                                                                        such that all elements meeting the criteria val<=elem should pe placed first.
*                                   - upper_bound(firstIt, lastIt, val): same as above only that the condition is val<elem (val should be upper bound)
*                                   - equal_range(firstIt, lastIt, val): returns a pair of iterators [first, first_past) pointing to a range containing all elements equal to val. The input range must 
*                                                                        be partitioned with respect to the input val, as all elements elem < val must precede the other => partion with predicate elem < val. 
*                                                                        It is basically a pair of iter [lower-bound;upper_bound)
*                                   - binary_search(firstIt, lastIt, val): recursively seraches for val in the input range, that must be partitioned with pred val < elem, returning bool 
*
*    7.11 Minimux/maximum operations: - max(T a, T b)/max_element(firstIt, LastIt): returns the maximum value between the 2 args/in the given range
*                                     - min(T a, T b)/min_element(firstIt, LastIt): returns the maximum value between the 2 args/in the given range
*                                     - minmax(T a, T b)/minmax_element(firstIt, LastIt): returns a pair of values/iterators to the min and the maximum value between the 2 args/in the given range
*
*    7.12 Merge operations: performed on sorted ranges
*                           - merge(firstIt, lastIt, firstIt2, lastIt2, outputFirstIt): merges the 2 ranges [firstIt, lastIt) and [firstIt2, lastIt2) into a single sorted
*                                                                                       range stored at outputFirstIt. Returns iterator past last element copied in the 
*                                                                                       new range
*                          - inplace_merge(firstIt, midIt, lastIt): Merges two consecutive sorted ranges [first, middle) and [middle, last) into one sorted range 
*                                                                  [first, last), returns void.
*
*    7.13 Set operations: - includes(first1, last1, first2, last2): Returns true if the sorted range [first2, last2) is a subsequence of the sorted range [first1, last1), 
*                                                                    a subsequence need not be contiguous, both ranges must be sorted in ascending order.
*                         - set_union(first1, last1, first2, last2, outputIt): constructs a sorted union beginning at outputIt consisting of the set of elements present 
*                                                                               in one or both sorted ranges [first1, last1) and [first2, last2). Returns iterator past 
*                                                                               last element in output range, the latter being mostly a sequence container
*                         - set_intersection(first1, last1, first2, last2, outputIt): constructs a sorted range beginning at outputIt consisting of the set of elements in 
*                                                                                  both sorted ranges [first1, last1) and [first2, last2). Returns iterator past last 
*                                                                                  element in output range, the latter being mostly a sequence container
*                         - set_difference(first1, last1, first2, last2, outputIt): constructs a sorted range beginning at outputIt consisting of the set of elements  
*                                                                                   found in range [first1, last1) but not in [first2, last2). Returns iterator past last 
*                                                                                   element in output range, the latter being  mostly a sequence container
*                         - set_symmetric_difference(first1, last1, first2, last2, outputIt): constructs a sorted range beginning at outputIt consisting of the set of 
*                                                                                              elements either found in range [first1, last1) or in range [first2, last2).
*                                                                                              Returns iterator past last element in output range, the latter being mostly  a sequence container
*
*
*     7.14 Max heap operations: - make_heap(firstIt, lastIt): creates a heap with the elements ordered in descending order (max heap), from the given range, which can be 
*                                                           any container. Returns void. It is not totally ordered, but partially, as it's underlying representation is
*                                                           a complete binary tree, with every node's value being larger or equal to its children values.
*                               - push_heap(firstIt, lastIt): inserts the element at the position last - 1 into the heap [first, last - 1). The heap after the insertion 
*                                                              will be [first, last). Normally, called after an element is pushed/added into the underlying container, 
*                                                              with push_heap reorganizing the tree, thus the elements. Handles one element at time => no init lists
*                               - sort_heap(firstIt, lastIt): converts the heap [first, last) into a sorted range. The heap property is no longer maintained. Returns void.
*                               - is_heap(firstIt, lastIt): Checks whether [first, last) is a heap. Returns bool
*                               - pop_heap(firstIt, lastIt): Swaps the value in the position first and the value in the position last - 1 and makes the subrange 
*                                                           [first, last - 1) into a heap. This has the effect of removing the first element from the heap [first, last)
* 
*    8. <numeric> header: defines numeric operations
*                       - void iota(firstIt, lastIt, init): fills the input range with values in ascending order, starting with init
*                       - accumulate(firstIt, lastIt, init): returns the sum of the elements in the range [first, last), computed starting with initial sum init
*                         accumulate(firstIt, lastIt, init, op): op is binary operation function object that will be applied to init, as it is iterated over range
*                       - inner_product(first1, last1, first2, init): computes sum of products or performs ordered map/reduce operation on the range [first1, last1) 
*                                                                   and the range beginning at first2
*                       - inner_product(first1, last1, first2, init, op1, op2): op2("prod") is the 2nd operand of op1 ("sum"), as init is the 1st operand at the 
*                                                                               1st call
*                       - adjacent_difference(firstIt, lastIt, outputIt): computes the differences between the second and the first of each adjacent pair of its 
*                                                                         elements and writes the differences to the range beginning at outputIt + 1. Iterator to the 
*                                                                         element past the last element written
*                       - partial_sum(firstIt, lastIt, outputIt): computes the partial sums of the elements in its subranges and writes the sums to the range 
*                                                                   beginning at outputIt, returning iter past last elem
*
*   9. <functional>: - plus<>(), minus<>(), divides<>(), multiplies<>(), modulus<>(), negate<>() template functor structs, that need to be instantiated before using ()
*                    - equal_to<>(), not_equal_to<>(), less<>(), less_equal<>(), greater<>(), greater_equal<>() template functor structs
*/
