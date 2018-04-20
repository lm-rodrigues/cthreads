#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/queues.h"

/* Inicia variáveis de controle e cria a thread main
   Retorna 0 se conseguiu, e -1 caso contrário */
int init_lib(void){
  TCB_t* main_thread;

  /* Inicia o controle das filas */
  FILA2 queue_all_threads;
  FILA2 queue_able;
  FILA2 queue_able_suspended;
  FILA2 queue_releaser_threds;

  control.all_threads = &queue_all_threads;
  control.able = &queue_able;
  control.able_suspended = &queue_able_suspended;
  control.releaser_threads = &queue_releaser_threds;

  /* Inicia as filas de controle e verifica se conseguiu
     Explora o comportamento de curto-circuito para melhor desempenho */
  if ( CreateFila2(control.all_threads) && CreateFila2(control.able) &&
       CreateFila2(control.able_suspended) && CreateFila2(control.releaser_threads)){
    return ERROR;
  }

  /* Define a biblioteca como inicializada e o
     último tid como 0 (Main Thread) */
  control.init = TRUE;
  control.last_created_tid = 0;

  /* Cria thread principal com TID = 0*/
  main_thread = (TCB_t*)malloc(sizeof(TCB_t));
  main_thread->state = PROCST_EXEC;
  main_thread->tid = 0;

  /* Insere thread principal em all_threads */
  AppendFila2(control.all_threads, main_thread);

  /* Cria funções de finalização para as threads */
  getcontext(&control.ended_thread);
  control.ended_thread.uc_link = NULL;
  control.ended_thread.uc_stack.ss_sp = (char*) malloc(STACK_SIZE);
  control.ended_thread.uc_stack.ss_size = STACK_SIZE;
  makecontext(&control.ended_thread, (void (*)(void))ended_thread, 0);

  /* Define Main thread como executando */
  control.running_thread = main_thread;

  /* Cria o context para main thread */
  getcontext(&main_thread->context);
  main_thread->context.uc_link = &control.ended_thread;
  main_thread->context.uc_stack.ss_sp = (char*) malloc(STACK_SIZE);
  main_thread->context.uc_stack.ss_size = STACK_SIZE;

  return 0;
}

/* Faz a troca para a thread que será executada
   Retorna 0 se conseguiu, -1 caso contrário */
int dispatcher(){
  TCB_t* next_thread;
  TCB_t* current_thread = control.running_thread;

  /* Pega a primeira thread da FIFO */
  if (FirstFila2(control.able))
    return ERROR;
  next_thread = GetAtIteratorFila2(control.able);

  /* Nenhuma thread pode ser executado*/
  if (next_thread == NULL)
    next_thread = current_thread;
  else {
    /* Remove a próxima thread da FIFO */
    if (DeleteAtIteratorFila2(control.able))
      return ERROR;
    /* Define como running_thread*/
    next_thread->state = PROCST_EXEC;
    control.running_thread = next_thread;
  }

  /* Thread que estava executando precisa ser inserida na fila de APTO */
  if (current_thread->state == PROCST_EXEC){
    /* Se a próxima thread é a thread executando não faz nada */
    if (next_thread != current_thread){
      /* Define o estado da thread antiga como APTO */
      current_thread->state = PROCST_APTO;
      AppendFila2(control.able, current_thread);
      /* Faz o swap do context para a nova thread */
      swapcontext(&current_thread->context, &next_thread->context);
    }
  }

  /* Thread antiga precisa ser bloqueada */
  else if (current_thread->state == PROCST_BLOQ){
    /* Faz o swap do context para a nova thread */
    swapcontext(&current_thread->context, &next_thread->context);
  }

  /* Thread executando precisa ser deletada */
  else if (current_thread->state == PROCST_TERMINO){
    /* Remove a thread executando da fila de all_threads */
    if (DeleteFromFila2(control.all_threads, current_thread))
      return ERROR;
    /* Libera o TCB da thread antigo da memoria */
    free(current_thread->context.uc_stack.ss_sp);
    free(current_thread);
    /* Se a próxima thread é a thread executando não faz nada*/
    if (next_thread != current_thread)
      setcontext(&next_thread->context);
  }

  return 0;
}

/* Rotina a ser executada sempre que uma thread terminar sua execução. */
void ended_thread(void){
  /* Atualiza contexto para proxima chamada*/
  getcontext(&control.ended_thread);

  /* Verifica se esta é uma RELEASER_THREAD*/
  release_verification();

  control.running_thread->state = PROCST_TERMINO;
  dispatcher();

  /* Verifica se não existem mais threads*/
  if (GetAtIteratorFila2(control.all_threads) == NULL)
    cclean();
}

/* Verifica se uma thread que chegou ao final de sua execução
   possui outra thread aguardando seu final. */
void release_verification(void){
  csem_t* sem;

  /* Procura a thread que chegou ao final em releaser threads. */
  sem = SearchReleaserThreadSemaphore(control.releaser_threads, control.running_thread->tid);

  /* Caso não haja nenhuma thread esperando pela thread finalizada. */
  if (sem != NULL) {
    /* Desbloqueia thread que está aguardando pela thread finalizada. */
    csignal(sem);

    /* Remove a thread finalizada de releaser threads. */
    DeleteReleaserThread(control.releaser_threads, control.running_thread->tid);
    free(sem);
  }
}

void cclean(){
  DestroyFILA2(control.all_threads);
  DestroyFILA2(control.able);
  DestroyFILA2(control.able_suspended);
  DestroyFILA2(control.releaser_threads);

}
