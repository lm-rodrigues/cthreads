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
  sem->count = count;
  if (CreateFila2(sem->fila))
    return ERROR;
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
