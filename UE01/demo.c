//-----------------------------------------------------------------------------
// demo.c
//
//
//                Terminal öffnen: Strg+Alt T
// 
// mit gcc compilieren und linken: gcc demo.c -o demo -Wall
//
//             executable starten: ./demo
//
//-----------------------------------------------------------------------------

#include <stdio.h>      // für printf
#include <stdlib.h>     // für EXIT_SUCCESS


int main(void)
{
	int i = 42;
	double x = 42.42;
	char s[] = "Fritzzzzzz";

	printf("i: %d, %-8o, %4x\n", i, i, i);
	printf("x: %-8.3f, %7.1e, %g\n", x, x, x);
	printf("Moin %s, Moin %10.5s", s, s);

	return EXIT_SUCCESS; // EXIT_SUCCESS = 0
}

