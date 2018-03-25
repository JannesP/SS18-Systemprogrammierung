#include <cstdio>
#include <stdlib.h>

struct Node {
	Node* next;
	int value;
};

int main()
{
	struct Node* node1 = (struct Node*)malloc(sizeof(struct Node));
	node1->value = 1;

	struct Node* node2 = (struct Node*)malloc(sizeof(struct Node));
	node2->value = 2;

	struct Node* node3 = (struct Node*)malloc(sizeof(struct Node));
	node3->value = 3;

	node1->next = node2;
	node2->next = node3;
	node3->next = node1;

	struct Node* reference = node2;

	printf("1: %d", reference->next->next->value);

	free(node1);
	free(node2);
	free(node3);

    return EXIT_SUCCESS;
}