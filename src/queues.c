#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/queues.h"

/* Delete a thread from a given queue
   Return 0 if susceded, -1 if not. */
int DeleteFromFila2(PFILA2 fila, TCB_t *tcb){
  TCB_t* tcb_aux;

  /* Set the iterator on queue init */
  if(FirstFila2(fila))
    return -1;
  /* Iterates over the queue and remove the node */
  while((tcb_aux=GetAtIteratorFila2(fila))) {
    /* Verify if found the node to remove */
    if (tcb_aux->tid == tcb->tid) {
      if (DeleteAtIteratorFila2(fila))
	return ERROR;
      else
	return 0;
    }
    /* Move to the next iterator */
    else if (NextFila2(fila))
      return ERROR;
  }
  return 0;
}
