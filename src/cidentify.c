#include "../include/cthread.h"

int cidentify (char *name, int size)
{
	char nomes[] = "Arthur L. Fuchs, 00261577 \nGabriel Martins, 00242288 \nLeonardo Marques Rodrigues, 00213751\n";
	int i = 0;
	while (i < size)
	{
		name[i] = nomes[i];
		if (nomes[i]=='\0')
		{
			break;
		}
		i++;
	}
	return 0;
}
