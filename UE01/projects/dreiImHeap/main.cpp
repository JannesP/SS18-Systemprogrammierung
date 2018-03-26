#include <cstdio>
#include <stdlib.h>

int main()
{
	char**** chainStart = (char****)malloc(sizeof(char));
	*chainStart = (char***)malloc(sizeof(char));
	**chainStart = (char**)malloc(sizeof(char));
	***chainStart = (char*)malloc(sizeof(char));
	****chainStart = 'X';

	printf("1:\t%p\t\t%p\n", &chainStart, chainStart);
	printf("2:\t%p\t\t%p\n", &*chainStart, *chainStart);
	printf("3:\t%p\t\t%p\n", &**chainStart, **chainStart);
	printf("4:\t%p\t\t%p\n", &***chainStart, ***chainStart);
	printf("4:\t%p\t\t%c", &****chainStart, ****chainStart);

    return EXIT_SUCCESS;
}