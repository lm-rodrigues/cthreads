#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/queues.h"


/* Init control variables and create main thread
   Return 0 if susceded, -1 if not. */
int init_lib(void){
  TCB_t* main_thread;

  /* Init control queues */
  FILA2 fila_all_thread;
  FILA2 fila_apto;
  FILA2 fila_apto_suspenso;

  control.all_threads = &fila_all_thread;
  control.apto = &fila_apto;
  control.apto_suspenso = &fila_apto_suspenso;

  /* Init all control queues and verify if succeded.
     exploits short-circuit behaviour to better performance. */
  if ( CreateFila2(control.all_threads) && CreateFila2(control.apto) &&
       CreateFila2(control.apto_suspenso) ){
    return ERROR;
  }

  /* Set the library as initialized */
  control.init = TRUE;
  /*Set last tid as 0 (Main Thread) */
  control.last_created_tid = 0;

  /* Create main thread with TID = 0 */
  main_thread = (TCB_t*)malloc(sizeof(TCB_t));
  main_thread->state = PROCST_EXEC;
  main_thread->tid = 0;

  /* Insert main thread in all_threads*/
  AppendFila2(control.all_threads, main_thread);

  /* Create end functions to threads*/
  getcontext(&control.ended_thread);
  control.ended_thread.uc_link = NULL;
  control.ended_thread.uc_stack.ss_sp = (char*) malloc(STACK_SIZE);
  control.ended_thread.uc_stack.ss_size = STACK_SIZE;
  //TODO: IMPLEMENT ENDED_THREAD
  makecontext(&control.ended_thread, (void (*)(void))ended_thread, 0);

  /* Set Main thread as running*/
  control.running_thread = main_thread;

  /* Create context to main thread*/
  getcontext(&main_thread->context);
  main_thread->context.uc_link = &control.ended_thread;
  main_thread->context.uc_stack.ss_sp = (char*) malloc(STACK_SIZE);
  main_thread->context.uc_stack.ss_size = STACK_SIZE;

  return 0;
}

/* Swap threads that should execute
   Return 0 if susceded, -1 if not. */
int dispatcher(){
  TCB_t* next_thread;
  TCB_t* current_thread = control.running_thread;

  /* Get the first thread of FIFO */
  if (FirstFila2(control.apto))
    return ERROR;
  next_thread = GetAtIteratorFila2(control.apto);

  /* No thread in able threads*/
  if (next_thread == NULL)
    next_thread = current_thread;
  else {
    /* Remove next thread from FIFO */
    if (DeleteAtIteratorFila2(control.apto))
      return ERROR;
    /* Set as running_thread*/
    next_thread->state = PROCST_EXEC;
    control.running_thread = next_thread;
  }

  /* Old running thread must be inserted in APTO queue */
  if (current_thread->state == PROCST_EXEC){
    /* If the next thread are the running thread do nothing*/
    if (next_thread != current_thread){
      /* Set state of old running thread as APTO and add to able threads*/
      current_thread->state = PROCST_APTO;
      AppendFila2(control.apto, current_thread);
      /* Swapping context to new thread*/
      swapcontext(&current_thread->context, &next_thread->context);
    }
  }

  /* Old running thread must be blocked*/
  else if (current_thread->state == PROCST_BLOQ){
    /* Swapping context to new thread*/
    swapcontext(&current_thread->context, &next_thread->context);
  }

  /* Running Thread must be deleted*/
  else if (current_thread->state == PROCST_TERMINO){
    /* Remove running thread from list of all threads*/
    if (DeleteFromFila2(control.all_threads, current_thread))
      return ERROR;
    /* Free TCB memory of old running thread*/
    free(current_thread->context.uc_stack.ss_sp);
    free(current_thread);
    /* If the next thread are the running thread do nothing*/
    if (next_thread != current_thread)
      setcontext(&next_thread->context);
  }

  return 0;
}
