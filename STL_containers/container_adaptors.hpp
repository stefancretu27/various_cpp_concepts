#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <tuple>
//for pair
#include <utility>

#include <list>
#include <vector>
#include <deque>

//container adaptors containers
#include <queue>
#include <stack>

/*
 * These data structures act as wrappers over certain sequence containers. 
 * As each of these data structure has its own charactertics, it implements a certain set of operations which wrap some existing operations in the underlying container, whereas the unwrapped ones are discarded.
 */ 

/*
 * The underlying container must provide back() access and push_back(), pop_back() modifiers
 * Thus, deque(default), list and vector can be used.
 */ 
void stack_operations();
/*
 * The underlying conatiner must provide front() and back() access and push_back(), pop_front() modifiers
 * Thus, deque(default) and list can be used.
 */
void queue_operations();
/*
 * A queue that orders the internally stored values in descending order, such that it provides constant time lookup to the largest element, that is always the first in its FIFO system
 * The underlying conatiners can only be deque and vector(default), as they need to provide random element access, front() access and push_back(), pop_back() modifiers
 */ 
void priority_queue_operations();
