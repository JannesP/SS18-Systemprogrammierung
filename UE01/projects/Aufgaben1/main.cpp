#include <cstdio>
#include <stdlib.h>

int main()
{
	int iFeld[2] = { 1, 2 };
	double dFeld[2] = { 1.0, 2.0 };

	printf("iFeld[0]: %d at %p\n", *iFeld, iFeld);
	printf("iFeld[1]: %d at %p\n\n", *(iFeld + 1), iFeld + 1);
	printf("int size: %ld\n\n\n", (long)(iFeld + 1) - (long)iFeld);

	printf("dFeld[0]: %f at %p\n", *dFeld, dFeld);
	printf("dFeld[1]: %f at %p\n\n", *(dFeld + 1), dFeld + 1);
	printf("double size: %ld\n", (long)(dFeld + 1) - (long)dFeld);

	return EXIT_SUCCESS;
}