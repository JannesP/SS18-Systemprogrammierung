#include <stdio.h>
#include <stdlib.h>

int main()
{
	float* pSchwelle = (float*)malloc(sizeof(float) * 3);
	*pSchwelle = 22.5;
	float* pKg = pSchwelle + 1;
	float* pSize = pKg + 1;

	printf("Gewicht in kg eingeben: ");
	scanf("%f", pKg);
	printf("Koerpergroesse eingeben: ");
	scanf("%f", pSize);

	printf("\n");

	if (*pKg / (*pSize * *pSize) < *pSchwelle) {
		printf("Spargel");
	}
	else {
		printf("Mops");
	}

	free(pSchwelle);
	return EXIT_SUCCESS;
}