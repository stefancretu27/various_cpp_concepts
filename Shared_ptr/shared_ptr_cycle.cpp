#include "shared_ptr.hpp"
#include <memory>
#include <iostream>

using namespace std;

typedef struct node
{
	int data;
	shared_ptr<struct node> next;
}s_node;

typedef struct w_node
{
	int data;
	weak_ptr<struct w_node> next;
}w_node;

void create_shared_ptr_list()
{
	s_node n1, n3;
	//create shared ptr from a previously declared object, using make-shared and copy assignment
	shared_ptr<s_node> s1 = make_shared<s_node>(n1);
	//create shared ptr to a dinamically allocated object created in shared ptr's c-tor 
	shared_ptr<s_node> s2(new s_node);
	
	//get the raw pointer (as const, but it can be not const) to the managed object 
	s_node * const ptr = s2.get();
	cout<<" raw pointer: "<<(ptr==nullptr)<<endl;
	
	s1->data = 0;
	s1->next = s2;
	s2->data = 1;
	{
		//copy c-tor with make shared
		shared_ptr<s_node> s3(make_shared<s_node>(n3));
		
		s2->next = s3;
		s3->data = 2;
		s3->next = s1;
		cout<<"count in inner scope: "<<s3.use_count()<<" data: "<<s3->data<<std::endl;
		
		//here s3 goes out of scope and its d-tor is called to decrement shared counter. The managed object won't be deleted as s2->next points to it
	}

	cout<<"count in outer scope: "<<s2->next.use_count()<<" data: "<<s2->next->data<<std::endl;
}


void create_weak_ptr_list()
{
	w_node w1,w2,w3;
	
	shared_ptr<w_node> ws1 = make_shared<w_node>(w1);
	shared_ptr<w_node> ws2(make_shared<w_node>(w2));
	shared_ptr<w_node> ws3 = make_shared<w_node>(w3);
	
	ws1->data = 0;
	ws1->next = ws2;
	ws2->data = 1;
	ws2->next = ws3;
	ws3->data = 2;
	ws3->next = ws1;
	cout<<"count before setting ws3 to null: "<<ws3.use_count()<<std::endl;
	
	//reset the pointer to nullptr. The d-tor is called to decrement the shared counter and delete the managed object, as s2->next is weak ptr and only points to the manager object
	ws3.reset();
	
	//use_count can also be used on weak_ptr and returns the # of shared_ptr
	cout<<"count after setting ws3 to nullptr: "<<ws2->next.use_count()<<" --------is the managed object expired?  "<<bool_to_word(ws2->next.expired())<<std::endl;
	//lock checks the manager object to see if the managed object still exists. If not, it returns a null shared_ptr
	cout<<"is empty the shared_ptr obtained using weak_ptr.lock()?  "<<bool_to_word((ws2->next.lock()==nullptr))<<std::endl;
	
	//weak ptr cannot be directly assigned to nullptr, but can be reset
	ws2->next.reset();
	
}
