#include <cstdio>
#include <stdlib.h>

struct Node {
	Node* next;
	int value;
};

int main()
{
	struct Node* start = (struct Node*)malloc(sizeof(struct Node));
	start->value = 1;
	start->next = (struct Node*)malloc(sizeof(struct Node));
	start->next->value = 2;
	start->next->next = (struct Node*)malloc(sizeof(struct Node));
	start->next->next->value = 3;
	start->next->next->next = start;


	start = start->next;

	printf("1: %d", start->next->next->value);

	return EXIT_SUCCESS;
}