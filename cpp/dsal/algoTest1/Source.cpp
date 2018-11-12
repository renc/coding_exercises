
struct Node {
	int data;
	Node *next;
};

Node * removeNodes(int iData, Node *head)
{
	if (head == nullptr)
		return nullptr;
	
	// find the first node (its data != iData)
	Node *newHead = head;
	while (newHead->data == iData)
	{
		Node *temp = newHead;
		newHead = newHead->next;
		delete temp;

		if (newHead == nullptr)
			return nullptr; // reach the end. 
	} 

	Node *prev = newHead;
	Node *curr = prev->next;
	while (curr != nullptr) {
		if (curr->data == iData)
		{
			prev->next = curr->next;
			 
			delete curr;
			curr = prev->next;
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}
	return newHead;
}