#include "Queue.hpp"

int main()
{
	Queue queue(0, 5);

	queue.append_node(1);
	queue.append_node(2);
	queue.print_queue();

	queue.prepend_node(-1);
	queue.print_queue();

	Node* node3 = new Node(3);
	queue.insert_node_at_index(node3, 2);
	queue.print_queue();

	queue.reverse_queue();
	queue.print_queue();

	queue.remove_first_node();
	queue.print_queue();

	queue.remove_node_at_index(2);
	queue.print_queue();

	queue.remove_last_node();
	queue.print_queue();

	return 0;
}