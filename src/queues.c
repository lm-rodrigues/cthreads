#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/queues.h"

/* Deleta uma thread de alguma lista
   Retorna 0 se conseguiu, e -1 caso contrário */
int DeleteFromFila2(PFILA2 fila, TCB_t *tcb){
  TCB_t* tcb_aux;

  /* Define o iterador na fila inicial */
  if(FirstFila2(fila))
    return -1;
  /* Itera na lista e remove nodos */
  while((tcb_aux=GetAtIteratorFila2(fila))) {
    /* Verifica se achou nodos para remover */
    if (tcb_aux->tid == tcb->tid) {
      if (DeleteAtIteratorFila2(fila))
	return ERROR;
      else
	return 0;
    }
    /* Move para o próximo iterador */
    else if (NextFila2(fila))
      return ERROR;
  }
  return 0;
}
