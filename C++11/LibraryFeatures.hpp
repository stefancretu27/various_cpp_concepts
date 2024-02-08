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
*               Mutex types: mutex, timed_mutex, recursive_mutex (used by the same thread, when resource is accessed recursively), recurisve_timed_mutex
*               Mutex wrappers, allow for owning mutexes in RAII style: lock_guard, unique_lock (offers interface for lock, unlock, try_lock operations => used with 
*               condition_variable), defer_lock (used with unique_lock, for more mutexes locked simultaneously), adopt_lock (used with lock_guard for same thing)
*
*   1.4 <condition_variable>: synch mechanism that blocks all threads waiting for being notified upon status change on the shared resource. Notification and
*               waiting operations are done via the condition_variable. Spurious wake-ups occurs when more threads are notified, one wakes-up, but another one
*               proceeds in accessing the shared resoruce => extra predicate function is used on waiting to mititgate its
*               condition_variable takes as 1st argument only unique_lock, whereas condition_variable_any works with any mutex type
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
*       7.1 Search operations: - find, find_if, find_if_not (firstIt, lastIt, val/pred/!pred). Value search/predicate-based search. Return iterator to firstly found occurence.
*                              - any_of, all_off, none_off(firstIt, lastIt, pred). Predicate-based search, return bool.
*                              - count, count_if(firstIt, lastIt, val/pred). Value/predicate-based search, return the number of iterators it in the range [first, last) satisfying the condition
*                              - search, find_end (firstIt, lastIt, s_first, s_last): search for 1st/last occurence of subrange [s_first, s_last) in range [firstIt, lastIt), returns iterator
*
*       7.2 Copy operations: - copy(firstIt, lastIt, output_firstIt), copy_if(..., pred), copy_n(firstit, count, output_firstIt), copy_backward, move, move_backward: return iterator to one past the last 
*                              copied elem.
*       7.3 Swap operations: - swap(var1, var2) => no return value, iter_swap (elem_it1, elem_it2) => no return value, swap_ranges (first1, last1, first2) swap containers, or subcontainers.
*                              Return Iterator to the element past the last element exchanged in the second range. 
*
*       7.4 Transformation operations: - std::transform(firstIt, lastIt, output_firstIt, func_object) applies the given function to a range and stores the result in another range, beginning at d_first
*                                       keeping the original elements order. Returns it to next to last transformed elem
*                                      - replace/replace_if(firstIt, lastIt, old_val/pred, new_val): replace all elements equal to old value/that satisfy the predicate, with the new value. No return value.
*
*       7.5 Generation operations: - fill/fill_n(firstIt, lastIt/size count, value): assign value to range/first n elems in range, no return
*                                  - generate/generate_n(firstIt, lastIt/size count, func_object): assign value generated by input function object to range/first n elems in range, no return 
*
*       7.6 Removing operations: - remove/remove_if(firstIt, lastIt, value/predicate): Removes all elements from the range [firstIt, lastIt) that are equal to input value/satisfy input predicate,
*                               with the remaining elements being copied to the first positions, return iterator to element past remaining elements range
*                                - remove_copy/remove_copy_iffirstIt, lastIt, outputIt, value/predicate: copies elems fron input range to output range, removing those elems equal to input value/satisfying
*                               input predicate p, returns iterator to the element past the last element copied.
*                                - unique/unique_copy(firstIt, lastIt, output_firstIt): eliminates duplicate elements in consecutive sub ranges, shifting the remaining elems to the first positions and 
*                               returns iterator past last not removed element (past logical end). (1,1,2,3,3,4,5,5) -> (1,2,3,4,5, garbage, garbage)
*
*       7.7 Order changing operations: - reverse/reverse_copy(firstIt, lastIt, output_firstIt): reverses the order of values, eventually copying the result in another container. No return
*                                      - rotate/rotate_copy(firstIt, middleIt, lastIt, output_firstIt): places the elements in range [firstIt, middleIt) after the elements [middleIt, lastIt). For copy,
*                                       the rotated elements are copied in the container whose begin() is output_firstIt. Returns iterator
*
*       7.8 Partitioning operations: - is_partitioned(firstIt, lastIt, pred), returns bool as it checks if all elements in the range [firstIt, lastIt) that satisfy the predicate pred appear before 
*                                       all elements that don't.
*                                       partition_point(firstIt, lastIt, pred): returns the end of the first partition, that is, the first element that does not satisfy p, or lastIt if all satisfy
*                                       partition, partition_copy(firstIt, lastIt, pred, out_firstIt, out_secondIt): divides the range in 2 subranges, the first range contains elements that satisfy pred, 
*                                       the second range doesn't eventually copying them to 2 distinct ranges (true range/false range). Returns iter to 1st elem of the second group/pair of iter to past last
*                                       elem in each range
*
*       7.9 Sorting operations: - sort(firstIt, lastIt), sorts elements in range, by default in ascending order, in O(n*logn)=>qsort. Equal elements' order might not be preserved
*                               - partial_sort(firstIt, midIt, lastIt), sorts elements in range [firstIt, midIt), by default in ascending order
*                               -  is_sorted(firstIt, lastIt), is_sorted_until(firstIt, lastIt): check if the range/the largest range is sorted in ascending order. Returna bools/iterator
*/
