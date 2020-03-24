#include "Queue.hpp"

using namespace std;

unsigned int Queue::queue_size = 0;
unsigned int Queue::queue_max_size;

Queue::Queue(int head_val, unsigned int max_size)
{
	//dynamically allocate the head node. Call Node c-tor to set the head's members
	this->head = new Node(head_val);
		
	//now check if the allocation was successful
	if(this->head == nullptr)
	{
		exit(0);
	}
	else 
	{
		//set max size
		queue_max_size = max_size;

		if(queue_size < queue_max_size)
		{
			//increase queue size
			queue_size++;
			cout<<"The head node was created. The queue's size is "<< queue_size<<endl;
		}
	}
	
}

Queue::~Queue()
{
	if(head)
	{
		Node* iterator = nullptr;
		Node* next = nullptr;
		unsigned short int index;
		const unsigned short int size = queue_size;

		cout<<endl<<"Queue destructor: all nodes in queue are deleted sequentially"<<endl;
		for(iterator = head, index = 0; iterator != NULL && index < size; iterator = next, index++)
		{
			//hold the address of the next element in a swap, so the increment can be properly done
			next = iterator->getNext();

			queue_size--;
			cout<<"The node with index "<< index <<" was removed. The queue's size is: "<< queue_size << endl;
			//free the current element
			delete(iterator);
		}

		//set the first element to null
		head = NULL;

		cout<<"The queue has been deleted"<<endl;
	}
}

void Queue::print_queue()
{
	/*
	* Start with the node given as input, then make sure it is not NULL, then dereference it in order to access the next element in the list.
	* If the check iterator=>next != NULL is used, the last element is not accessed as itś next points to NULL
	*/
	if(head)
	{
		cout<<endl<<"The elements currently in the queue are listed beneath:" <<endl;
		for(Node* iterator = head; iterator != NULL; iterator = iterator->getNext())
		{
			cout<<"Pointer: "<<iterator<<" | Value: "<< iterator->getVal() <<" | Next pointer: "<<iterator->getNext()<<endl;
		}
		cout<<endl;
	}
}

void Queue::append_node(Node* &new_node)
{
	//append the new node only if the head exists
	if(head && queue_size < queue_max_size)
	{
		Node* iterator = nullptr;
		//start with the node given as input. If it points to another node, then dereference it in order to access the next element in the list
		for(iterator = head; iterator->getNext() != NULL; iterator = iterator->getNext());
		
		iterator->setNext(new_node);
		//it is the last element so its next is NULL and its makeprev is iterator (former last node)
		new_node->setNext(nullptr);
		new_node->setPrev(iterator);
		//node added to list, increase the list size
		queue_size++;
		cout<<"A new node was appended. The queue's size is: "<< queue_size<<endl;
	}
	else
	{
		cout<<" The new node was not added as the queue's size has reached the max limit"<<endl;
	}
}

void Queue::append_node(int node_val)
{
	//append the new node only if the head exists
	if(head && queue_size < queue_max_size)
	{
		//firstly, create the new node
		Node* new_node = new Node(node_val);

		Node* iterator = nullptr;
		//start with the node given as input. If it points to another node, then dereference it in order to access the next element in the list
		for(iterator = head; iterator->getNext() != NULL; iterator = iterator->getNext());
		
		//last element found: double check
		iterator->setNext(new_node);
		//it is the last element so its next is NULL and its makeprev is iterator (former last node)
		new_node->setNext(nullptr);
		new_node->setPrev(iterator);
		//node added to list, increase the list size
		queue_size++;
		cout<<"A new node was appended. The queue's size is: "<< queue_size<<endl;
	}
	else
	{
		cout<<" The new node was not added as the queue's size has reached the max limit"<<endl;
	}
}

void Queue::remove_last_node()
{
	if(head)
	{
		//if the list contains only the head
		if(head->getNext() == nullptr)
		{
			delete(head);
			head = nullptr;
			queue_size--;
			cout<<"The queue's head was removed. The queue's size is: "<< queue_size<<endl;
		}
		else
		{
			Node* iterator;
			//find the second last element in list
			for(iterator = head; iterator->getNext()->getNext() != nullptr; iterator = iterator->getNext());

			//double check
			if(iterator->getNext()->getNext() == NULL)
			{
				//second last element was found, delete the node it points to
				delete(iterator->getNext());
				iterator->setNext(NULL);

				cout<<"The last node was removed. The new tail address is "<<iterator<<" its value is "<<iterator->getVal();
				queue_size--;
				cout<<"The queue's size is: "<< queue_size<<endl;
			}
		}
	}
}

void Queue::prepend_node(Node* &new_node)
{
	if(head && queue_size < queue_max_size)
	{
		//by default, any new node does not point to next and prev nodes, but to null. New node should point to old head
		new_node->setNext(head);
		head->setPrev(new_node);

		//replace head with the new node
		head = new_node;

		//node added to list, increase the list size
		queue_size++;
		cout<<"A new node was prepended. The queue's size is: "<< queue_size<<endl;
	}
	else
	{
		cout<<" The new node was not added as the queue's size has reached the max limit"<<endl;
	}
	
}

void Queue::prepend_node(int node_val)
{
	if(head && queue_size < queue_max_size)
	{
		//firstly, create the new node
		Node* new_node = new Node(node_val);

		//by default, any new node does not point to next and prev nodes, but to null. New node should point to old head
		new_node->setNext(head);
		head->setPrev(new_node);

		//replace head with the new node
		head = new_node;

		//node added to list, increase the list size
		queue_size++;
		cout<<"A new node was prepended. The queue's size is: "<< queue_size<<endl;
	}
	else
	{
		cout<<" The new node was not added as the queue's size has reached the max limit"<<endl;
	}
}

void Queue::remove_first_node()
{
	if(head)
	{
		//if there is only one element in list
		if(head->getNext() == nullptr)
		{
			//delete the head
			delete(head);
			head = nullptr;
			//update list size
			queue_size--;
			cout<<"The list's head was removed. The queue's size is: "<< queue_size<<endl;
		}
		else
		{
			//get second element
			Node* new_head = head->getNext();

			//delete first element
			delete(head);
			//once the old head is deleted, new heads prev is NULL
			new_head->setPrev(nullptr);

			//replace head with the new node
			head = new_head;
			cout<<"The first node was removed. The new head address is "<< head <<" its value is "<<head->getVal()<<endl;
			
			//update list size
			queue_size--;
			cout<<"The queue's size is: "<< queue_size<<endl;
		}
	}
}

void Queue::insert_node_at_index(Node* &new_node, unsigned short int index)
{
	if(head && index <= queue_size)
	{
		//if the node is inserted on the first position, then it is prepended
		if(index == 0)
		{
			prepend_node(new_node);
		}
		//if the node is appended, it is added on index ehich is the current list_size + 1
		else if(index == queue_size)
		{
			append_node(new_node);
		}
		else
		{
			Node *iterator = nullptr, *swap = nullptr;
			unsigned short int idx = 0;

			//Within the loop is seeked for the node pointing to the node whose next should be the new node
			//After the loop ends its iterations both the iterator and index will indicate the node to whom the new node shall be appended
			for(iterator = head, idx = 0; iterator->getNext() != nullptr && idx < index - 1; iterator = iterator->getNext(), idx++);

			//take next node
			swap = iterator->getNext();

			iterator->setNext(new_node);
			new_node->setNext(swap);
			swap->setPrev(new_node);

			queue_size++;
			cout<<"A new node was added on position "<<index <<" The queue's size is: "<<queue_size<<endl;
		}
		
	}
}

void Queue::remove_node_at_index(unsigned short int index)
{
		if(head && index <= queue_size)
	{
		//if the node is inserted on the first position, then it is prepended
		if(index == 0)
		{
			remove_first_node();
		}
		//if the node is appended, it is added on index ehich is the current list_size + 1
		else if(index == queue_size-1)
		{
			remove_last_node();
		}
		else
		{
			Node *iterator = nullptr, *swap = nullptr;
			unsigned short int idx = 0;

			//Within the loop is seeked for the node pointing to the node whose next should be the node to be deleted
			//After the loop ends its iterations both the iterator and index will indicate the node to whom the new node shall be appended
			for(iterator = head, idx = 0; iterator->getNext() != nullptr && idx < index - 1; iterator = iterator->getNext(), idx++);

			//get the next node that the node to be deleted points to
			swap = iterator->getNext()->getNext();
			delete(iterator->getNext());
			iterator->setNext(swap);
			swap->setPrev(iterator); 

			queue_size--;
			cout<<"The node on position "<< index <<" was deleted. The queue's size is: "<< queue_size<<endl;
		}
		
	}
}

void Queue::reverse_queue()
{
	Node* iterator = this->head;
	Node* next_node = NULL;
	Node* prev = NULL;

	while(iterator)
	{
		//store the next node in list in a separate memory, useful for next iteration
		next_node = iterator->getNext();
		//set the current node to point to the previous one
		iterator->setNext(prev);
		//the current node in queue will be the previous of the next one
		prev = iterator;
		//go to next element in list
		iterator = next_node;
	}

	head = prev;
	cout<<"The queue was reversed"<<endl;
}

/*
void delete_stack(node_t** tail)
{
	if(*tail)
	{
		node_t *iterator, *next;
		unsigned short int index;
		const unsigned short int size = (**tail).list_size;

		printf("All nodes in stack are deleted sequentially\n");
		for(iterator = *tail, index = 0; iterator != NULL && index < size; iterator = next, index++)
		{
			//take the address of the prev element
			next = iterator->prev;

			node_t::list_size--;
			printf("The node %d was removed. The stack size is: %d\n", index,  node_t::list_size);
			//free the current element
			free(iterator);
		}

		*tail = NULL;

		printf("The stack has been deleted\n");
	}
}
*/