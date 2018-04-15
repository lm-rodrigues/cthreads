#include "../include/cthread.h"
#include "../include/cdata.h"

/******************************************************************************
Parâmetros:
	name:	ponteiro para uma área de memória onde deve ser escrito um string que contém os nomes dos componentes do grupo e seus números de cartão.
		Deve ser uma linha por componente.
	size:	quantidade máxima de caracteres que podem ser copiados para o string de identificação dos componentes do grupo.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int cidentify (char *name, int size) {
#define GROUP_NAMES_SIZE 92
  
  char group_names[] = "Arthur L. Fuchs, 00261577 \nGabriel Martins, 00242288 \nLeonardo Marques Rodrigues, 00213751\n";
  int i = 0;

  if ( !name ) {
    return ERROR;
  }
  else {
    while (i < size-1 && group_names[i] != '\0') {
      name[i] = group_names[i];
      i++;
    }

    name[i] = '\0';
    return 0;

  }
}
