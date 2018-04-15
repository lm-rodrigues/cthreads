/*
 * cdata.h: arquivo de inclusï¿½o de uso apenas na geraï¿½ï¿½o da libpithread
 *
 * Esse arquivo pode ser modificado. ENTRETANTO, deve ser utilizada a TCB fornecida.
 * Se necessário, pode-se ACRESCENTAR campos ao TCP
 *
 * VERSÃO: 17/03/2018
 *
 */
#ifndef __cdata__
#define __cdata__
#include "ucontext.h"
#include "support.h"

#define	PROCST_CRIACAO	0
#define	PROCST_APTO	1
#define	PROCST_EXEC	2
#define	PROCST_BLOQ	3
#define	PROCST_TERMINO	4
#define	PROCST_APTO_SUS	5
#define	PROCST_BLOQ_SUS	6

#define ERROR -1

#define TRUE 0
#define FALSE -1

#define STACK_SIZE 4096

/* Os campos "tid", "state" e "context" dessa estrutura devem ser mantidos e usados convenientemente
   Pode-se acrescentar outros campos APÓS os campos obrigatórios dessa estrutura
*/
typedef struct s_TCB { 
	int		tid; 		// identificador da thread
	int		state;		// estado em que a thread se encontra
					// 0: Criação; 1: Apto; 2: Execução; 3: Bloqueado; 4: Término; 5: Apto-Suspenso; 6: Bloqueado-Suspenso
	int 		prio;		// Prioridade associada a thread NÃO USADO NESSA IMPLEMENTAÇÃO
	ucontext_t 	context;	// contexto de execução da thread (SP, PC, GPRs e recursos) 
	
	/* Se necessário, pode-se acresecentar campos nessa estrutura A PARTIR DAQUI! */
	
	
} TCB_t; 

struct _cth{
  /* Verify if the main thread was initialized */
  int init;
  /* Tid, of last created thread */
  int last_created_tid;
  /* All threads that are being managed by the library */
  PFILA2 all_threads;
  /* All threads that are ready for execution */
  PFILA2 apto;
  /* All threads that are ready for execution but are suspended to disk */
  PFILA2 apto_suspenso;
  /* Thread that are being executed */
  TCB_t* running_thread;
  /* UCP to delete a thread that end */
  ucontext_t ended_thread;
};

extern struct _cth control;

/* Prototypes of the functions implemented by this module */

/* Return 0 if susceded, -1 if not. */
int init_lib(void);
void ended_thread(void);
int dispatch(void);

#endif
