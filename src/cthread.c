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

  /* Atribui a new_thread um TID e um estado inicial */
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
  // Checar se variaveis internas foram inicializadas
  if(control.init == FALSE)
    if(init_lib())
      return ERROR;

  // Inicializar o semáforo
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
  // Checar se variaveis internas foram inicializadas
  if(control.init == FALSE)
    if(init_lib())
      return ERROR;

  // Se o recurso estiver livre, decrementar
  if (sem->count > 0)
  {
    sem->count--;
    return 0;
  }
  // Caso contrário, colocar a thread na FIFO do semáforo
  else
  {
    if (AppendFila2(sem->fila, (void*)control.running_thread))
    {
      return ERROR;
    }
    // Bloquear thread e diminuir count do semáforo;
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
  // Checar se variáveis internas foram inicializadas
  if (control.init == FALSE)
    if(init_lib())
      return ERROR;
  
  TCB_t* blocked_thread;

  
  // Mover iterador para primeira thread da fila do semáforo
  if (FirstFila2(sem->fila))
  {
    // Em caso de erro, verificar se a fila está vazia
    if (NextFila2(sem->fila) == -NXTFILA_VAZIA)
    {
      // Incrementar count
      sem->count ++;
      return 0;
    }
    // Se não estiver, ocorreu um erro
    return ERROR;
  }

  blocked_thread = GetAtIteratorFila2(sem->fila);
  if (blocked_thread==NULL)
  {
    // Como a fila não está vazia ocorreu um erro de iterador
    return ERROR;
  }

  // Colocar a thread na fila de APTOS
  if (AppendFila2(control.apto, (void*) blocked_thread))
  {
    //se não conseguir, retornar erro
    return ERROR;
  }

  // Remover a thread da fila do semáforo 
  // -> Não deve ocorrer erro pois erros de iterador invalido e
  // fila vazia já foram tratados anteriormente.
  DeleteAtIteratorFila2(sem->fila, blocked_thread);

  // Mudar o estado da thread para APTO
  blocked_thread->state = PROCST_APTO;

  // Incrementar count
  sem->count ++;
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
