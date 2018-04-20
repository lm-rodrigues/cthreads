#include <stdlib.h>
#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/cthread.h"

/* Struct de controle Global */
struct lib_control control = {.init = FALSE};


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

  /* Checa se variáveis internas foram inicializadas */
  if(control.init == FALSE)
    if(init_lib())
      return ERROR;

  /* Cria thread context */
  new_thread = (TCB_t*) malloc(sizeof(TCB_t));
  getcontext(&new_thread->context);
  new_thread->context.uc_stack.ss_sp = (char*) malloc(STACK_SIZE);
  new_thread->context.uc_stack.ss_size = STACK_SIZE;
  new_thread->context.uc_link = &control.ended_thread;
  makecontext(&new_thread->context, (void (*)(void))start, 1, arg);

  /* Atribiu a new_thread um TID e um estadao inicial */
  control.last_created_tid++;
  new_thread->tid = control.last_created_tid;
  new_thread->state = PROCST_APTO;

  /* Coloca a new_thread na fila de APTO*/
  AppendFila2(control.all_threads, (void *) new_thread);
  AppendFila2(control.able, (void *) new_thread);

  /* Returna o Thread Identifier (TID) */
  return new_thread->tid;
}


/*
 Parâmetros:
   Sem parâmetros
 Retorno:
   Se correto => 0 (zero)
   Se erro    => Valor negativo. */
int cyield(void){

  /* Checa se variáveis internas foram inicializadas */
  if(control.init == FALSE)
    if(init_lib())
      return ERROR;

  return dispatcher();
}

/*
  Parâmetros:
    sem: ponteiro para estrutura de semáforo a ser inicializada
    count: numero de recursos que o semáforo controla
  Retorno:
    Se correto => 0 (zero)
    Se erro    => Valor negativo. */
int csem_init(csem_t *sem, int count)
{
  //checar se variaveis internas foram inicializadas
  if(control.init == FALSE)
    if(init_lib())
      return ERROR;

  //inicializar o semáforo
  sem->count = count;
  sem->fila = (PFILA2) malloc(sizeof(PFILA2));
  if (CreateFila2(sem->fila))
    return ERROR;
  return 0;
}

/*
  Parâmetros:
    sem: ponteiro para estrutura de semáforo
  Retorno:
    Se correto => 0 (zero)
    Se erro    => Valor negativo. */
int cwait(csem_t *sem)
{
  //checar se variaveis internas foram inicializadas
  if(control.init == FALSE)
    if(init_lib())
      return ERROR;

  //se o recurso estiver livre, decrementar.
  if (sem->count > 0)
  {
    sem->count--;
    return 0;
  }
  //caso contrário, colocar a thread na FIFO do semaforo
  else
  {
    if (AppendFila2(sem->fila, (void*)control.running_thread))
    {
      return ERROR;
    }
    //bloquear thread e diminuir count do semaforo;
    control->running_thread.state = PROCST_BLOQ;
    sem->count--;
    return dispatcher();    
  }

  return 0;
}

/*
  Parâmetros:
    sem: ponteiro para estrutura de semáforo
  Retorno:
    Se correto => 0 (zero)
    Se erro    => Valor negativo. */
inc csignal(csem_t* sem)
{
  //checar se variaveis internas foram inicializadas
  if (control.init == FALSE)
    if(init_lib())
      return ERROR;
  
  TCB_t* blocked_thread;

  //incrementar count
  sem->count ++;

  //mover iterador para primeira thread da fila do semaforo
  if (FirstFila2(sem->fila))
  {
    //em caso de erro, verificar se a fila está vazia.
    if (NextFila2(sem->fila) == -NXTFILA_VAZIA)
    {
      return 0;
    }
    //se não estiver, ocorreu um erro
    return ERROR;
  }

  blocked_thread = GetAtIteratorFila2(sem->fila);
  if (blocked_thread==NULL)
  {
    //como a fila não está vazia ocorreu um erro de iterador
    return ERROR;
  }

  //remover a thread da fila do semáforo
  DeleteAtIteratorFila2(sem->fila, blocked_thread);

  //mudar o estado da thread para apto
  blocked_thread->state = PROCST_APTO;
  //colocar a thread na fila de aptos
  return AppendFila2(control.apto, (void*) blocked_thread);
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
  char group_names[] = "Arthur L. Fuchs, 00261577 \nGabriel Fonseca Martins, 00242288 \nLeonardo Marques Rodrigues, 00213751\n";
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
