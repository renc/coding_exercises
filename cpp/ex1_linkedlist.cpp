#include <stdlib.h> // for NULL

// http://cslibrary.stanford.edu/103/LinkedListBasics.pdf by Nick Parlante.
struct node {
	int				data;
	struct node		*next;
};

int length(struct node *head)
{
	int count = 0; 
	struct node *current = head;
	while (current != NULL) {
		++count;
		current = current->next;
	}
	return count;
}

int main()
{
	struct node n0; 
	n0.data = 1;
	return 1;
}