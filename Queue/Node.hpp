class Node
{
private:
	int val;
	Node *next;
	Node *prev;

public:
	Node(int value);
	Node(const Node& new_node);
    Node(const Node* &new_node);

	//setters
	inline void setVal(const int value);
    //const reference of Node*(pointer to Node) type
	inline void setNext(Node* const &new_next);
	inline void setPrev(Node* const &new_prev);
	//getters
	inline int getVal() const;
	inline Node* getNext() const;
	inline Node* getPrev() const;

	//operators overloads
    void operator=(const Node& new_node);
    void operator=(const Node* &new_node);
};

//setters
inline void Node::setVal(const int value) 
{
    this->val = value;
};
inline void Node::setNext(Node* const &new_next)
{
    this->next = new_next;
};
inline void Node::setPrev(Node* const &new_prev) 
{
    this->prev = new_prev;
};

//getters
inline int Node::getVal() const
{
    return this-> val;
};
inline Node* Node::getNext() const
{
    return this->next;
 };
inline Node* Node::getPrev() const
{
    return this->prev;
};