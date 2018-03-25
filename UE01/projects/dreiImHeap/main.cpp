#include <cstdio>
#include <stdlib.h>

int main()
{
	char* value = (char*)malloc(sizeof(char));
	*value = 'X';
	char** ptr1 = (char**)malloc(sizeof(char));
	*ptr1 = value;
	char*** ptr2 = (char***)malloc(sizeof(char));
	*ptr2 = ptr1;
	char**** chainStart = &ptr2;

	printf("1:\t%p\t\t%p\n", chainStart, *chainStart);
	printf("2:\t%p\t\t%p\n", ptr2, *ptr2);
	printf("3:\t%p\t\t%p\n", ptr1, *ptr1);
	printf("4:\t%p\t\t%c", value, *value);

    return EXIT_SUCCESS;
}