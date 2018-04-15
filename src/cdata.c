#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/support.h"


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

