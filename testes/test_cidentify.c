#include <stdio.h>
#include "../include/cthread.h"

#define LEN 95

int main()
{
	char nomes[LEN];
	cidentify(nomes, LEN);
	printf("%s", nomes);
	return 0;
}
