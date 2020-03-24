#include<stdlib.h>
#include "iostream"

//own defined headers
#include "Node.hpp"

class Queue
{
private:
	Node *head;
	static unsigned int queue_size;
	static unsigned int queue_max_size;

public:
	Queue(int head_val, unsigned int max_size);
	~Queue();

	//setter
	inline void setHead(Node* new_head);
	//getters
	inline const Node* getHead() const;

	inline bool isQueueFull();
	inline bool isQueueEmpty();
	/*
 	 * Iterate over the list and find the last node, then append the new node to the last node in list
 	 * Input:
 	 *  - the node to be appended
 	 */ 
	void append_node(Node* &new_node);
	/*
 	 * Iterate over the list and find the last node, then append the new node to the last node in list
 	 * Input:
 	 *  - value of the node to be appended
 	 */ 
	void append_node(int node_val);
	/*
	* Replace head with the new node which becomes the new head. The new head points to old head
	* Input:
	*  - the node to be prepended (new head)
	*/ 
	void prepend_node(Node* &new_node);
	/*
	* Replace head with the new node which becomes the new head. The new head points to old head
	* Input:
	*  - value of the node to be prepended
	*/ 
	void prepend_node(int node_val);

	/*
	* The new head is the old second element
	*/ 
	void remove_first_node();
	/*
	 * Iterate over the list and find the second last node, then remove the next node
	 * Input:
	 *  - the head node of the list
	 */ 
	void remove_last_node();
	/*
	* If index equals lists size, then call append_node.
	* If index equals 0, then call prepend_node.
	* Else, iterate over the list to find the previous node to the one pointed by index.
	* Once found, then append the new node to it and prepend to its former next.
	* Input:
	*  - the node to be inserted
	*  - the index where the new node should be inserted 
	*/ 
	void insert_node_at_index(Node* &new_node, unsigned short int index);
	/*
	* If index equals lists size, then call remove_last_node.
	* If index equals 0, then call remove_first_node.
	* Else, iterate over the list to find the previous node to the one pointed by index.
	* Once found, then remove the node it points to and bind it to the deleted nodes next.
	* Input:
	*  - the head node of the list
	*  - the node to be inserted
	*  - the index where the new node should be inserted 
	*/
	void remove_node_at_index(unsigned short int index);

	/*
	*
	*/
	void reverse_queue();


	/*
	 * Iterate over the list and print the integer values and the current pointer value
	 */
	void print_queue();
	/*
	* Iterate over the list and delete the current element. Lastly set the head to point to NULL
	* Input:
	*  - the head node of the queue
	*/ 
	void delete_stack(Node* &tail);

};

//setter
inline void Queue::setHead(Node* new_head)
{
	this->head = head;
};
//getter
inline const Node* Queue::getHead() const 
{
	return this->head;
};

inline bool Queue::isQueueEmpty()
{
	return (head == nullptr)||(queue_size == 0);
}

inline bool Queue::isQueueFull()
{
	return queue_size == queue_max_size;
}