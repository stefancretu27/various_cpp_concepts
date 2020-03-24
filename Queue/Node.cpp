#include "Node.hpp"

Node::Node(int value)
{
	this->val = value;
	this->next = nullptr;
	this->prev = nullptr;
}

Node::Node(const Node &new_node)
{
    this->val = new_node.getVal();
	this->next = new_node.getNext();
	this->prev = new_node.getPrev();
}

Node::Node(const Node* &new_node)
{
    this->val = new_node->getVal();
	this->next = new_node->getNext();
	this->prev = new_node->getPrev();
}

void Node::operator=(const Node &new_node)
{
    this->val = new_node.getVal();
	this->next = new_node.getNext();
	this->prev = new_node.getPrev();
}

void Node::operator=(const Node* &new_node)
{
    this->val = new_node->getVal();
	this->next = new_node->getNext();
	this->prev = new_node->getPrev();
}