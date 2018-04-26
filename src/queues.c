#include <stdlib.h>
#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/queues.h"

/* Deleta uma thread de alguma fila
   Retorna 0 se conseguiu, e -1 caso contrário */
int DeleteFromFila2(PFILA2 fila, TCB_t *tcb){
  TCB_t* tcb_aux;

  /* Define o iterador no inicio da fila */
  if(FirstFila2(fila))
    return -1;

  /* Itera na lista e remove nodos */
  while((tcb_aux=GetAtIteratorFila2(fila))) {
    /* Verifica se achou o nodo a remover */
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

/* Procura um Releaser Thread e retorna o seu semaforo. */
csem_t* SearchReleaserThreadSemaphore(PFILA2 releaser_threads_queue, int tid){
  releaser_thread_control* rtc_aux;

  /* Define o iterador no inicio da fila */
  FirstFila2(releaser_threads_queue);

  /* Itera na lista procurando o nodo necessário */
  while((rtc_aux=GetAtIteratorFila2(releaser_threads_queue))) {
    /* Verifica se achou o nodo a retornar */
    if (rtc_aux->tid == tid)
      return rtc_aux->sem;
    else
      NextFila2(releaser_threads_queue);
  }
  return NULL;
}

/* Remove uma Releaser Thread de uma fila */
void DeleteReleaserThread(PFILA2 releaser_threads_queue, int tid){
  releaser_thread_control* rtc_aux;

  /* Define o iterador no inicio da fila */
  FirstFila2(releaser_threads_queue);

  /* Itera na lista e remove o nodo necessário */
  while((rtc_aux=GetAtIteratorFila2(releaser_threads_queue))) {
    if (rtc_aux->tid == tid)
      DeleteAtIteratorFila2(releaser_threads_queue);
    else
      NextFila2(releaser_threads_queue);
  }
}

/* Destrói uma fila */
void DestroyFILA2(PFILA2 queue){
  /* Define o iterador no inicio da fila */
  FirstFila2(queue);

  /* Itera na lista e remove nodos */
  while(DeleteAtIteratorFila2(queue)) {
      NextFila2(queue);
  }
  /* Desaloca a fila da memória. */
  free(queue->first);
  free(queue->last);
  free(queue->it);
  free(queue);
}

/* Procura uma thread eu uma fila */
TCB_t* searchThread(PFILA2 queue, int tid)
{
  TCB_t *tr;
  /* Define o iterador no inicio da fila */
  FirstFila2(queue);

  while (tr = GetAtIteratorFila2(queue))
  {
    if (tr->tid == tid)
    {
      return tr;
    }
    else
    {
      NextFila2(queue);
    }
  }
  return NULL;
}