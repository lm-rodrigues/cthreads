#include "../include/cdata.h"
#include "../include/support.h"

/* Remove uma thread de alguma fila.
   Retorna 0 se bem suscedida, -1 caso contrário. */
int DeleteFromFila2(PFILA2, TCB_t *);

/* Procura uma Releaser Thread pelo seu tid e retorna o seu semaforo. */
csem_t* SearchReleaserThreadSemaphore(PFILA2 releaser_threads_queue, int tid);

/* Remove uma Releaser Thread de uma fila. */
void DeleteReleaserThread(PFILA2 releaser_threads_queue, int tid);

/* Destrói uma fila. */
void DestroyFILA2(PFILA2 queue);

/*  Procura uma thread em uma fila. 
    Parametros: 
        queue: PFILA2 - fila a ser vasculhada
        tid : int - tid da thread a ser procurada
    Retorna: o ponteiro para o TCB da thread se achou
             um valor negativo se ocorrer um erro
                */
TCB_t* searchThread(PFILA2 queue, int tid);

