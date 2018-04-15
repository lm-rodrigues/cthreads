#include <stdlib.h>
#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/cthread.h"

/* Global control struct */
struct _cth control = {.init = FALSE};


/*
 Parâmetros:
	start:  ponteiro para a função que a thread executará.
	arg:    um parâmetro que pode ser passado para a thread na sua criação.
	prio:   NÃO utilizado neste semestre, deve ser sempre zero.
 Retorno:
	Se correto => Valor positivo, que representa o identificador da thread criada
	Se erro    => Valor negativo. */
int ccreate (void* (*start)(void*), void *arg, int prio){
	prio = 0;
  	TCB_t* new_thread;

  /* Check if internal variables was initialized */
	if(control.init == FALSE)
    	if(init_lib())
    		return ERROR;

	/* Making thread context */
	new_thread = (TCB_t*) malloc(sizeof(TCB_t));
	getcontext(&new_thread->context);
	new_thread->context.uc_stack.ss_sp = (char*) malloc(STACK_SIZE);
	new_thread->context.uc_stack.ss_size = STACK_SIZE;
	new_thread->context.uc_link = &control.ended_thread;
	makecontext(&new_thread->context, (void (*)(void))start, 1, arg);

	/* Assign to new_thread a TID and initial state */
	control.last_created_tid++;
	new_thread->tid = control.last_created_tid;
	new_thread->state = PROCST_APTO;

	/* Put new_thread it into proper queues */
	AppendFila2(control.all_threads, (void *) new_thread);
	AppendFila2(control.apto, (void *) new_thread);

	/* Return the Thread Identifier*/
	return new_thread->tid;
}

//TODO: testar cyield
/*
 Retorno: 
	Se correto => 0 (zero)
	se erro    => Valor negativo.
*/
int cyield(void)
{
	// Who is a good thread? It's the one that calls yield sometimes
	TCB_t *good_thread= control.running_thread;
	control.running_thread = NULL;

	// Try to dispatch a new thread to be exectuted
	if (dispatch()==0)
	{
		// The good thread goes back to queue
		good_thread -> state = PROCST_APTO;
		if (AppendFila2(control.apto, (void *) good_thread != 0)
		return ERROR;
	}
	else
	{
		control.running_thread = good_thread;
		return ERROR;
	}
	return 0;
}


/*
 Parâmetros:
   name: ponteiro para uma área de memória onde deve ser escrito um string que contém os nomes dos componentes do grupo e seus números de cartão.
         Deve ser uma linha por componente.
   size: quantidade máxima de caracteres que podem ser copiados para o string de identificação dos componentes do grupo.
 Retorno:
   Se correto => 0 (zero)
   Se erro    => Valor negativo. */
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
