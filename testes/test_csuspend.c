/*
 * teste_suspende: teste para verificar se csuspend e cresume estão funcionais.
 */

 #include	"../include/support.h"
 #include	"../include/cthread.h"
 #include	<stdio.h>
 #include	<stdlib.h>

void* func(void *arg)
{
  printf("thread ID0, imprimindo %d\n", *((int *)arg));
  return;
}

int main()
{
  int id0;
  int i = 100;

  id0 = ccreate(func, (void *)&i, 0);

  printf("Main\n");
  if (csuspend(id0))
  {
    printf("erro ao suspender thread ID0\n");
    return -1;
  }

  printf("thread ID0 suspensa\n");

  if (cresume(id0))
  {
    printf("erro ao resumir thread ID0\n");
    return -1;
  }

  printf("thread ID0 resumida\n");
  cjoin(id0);
  printf("fim do programa\n");

  return 0;
}
