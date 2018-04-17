# CTHREADS - Compact Threads

Biblioteca de threads em nível de usuário (N:1) que oferece
capacidades basicas de programação com threads, como criação,
execução, sincronização, término e trocas de contexto.

Os estados de transição possíveis para cada thread são os seguintes:

- APTO;
- EXECUTANDO;
- BLOQUEADO;
- APTO-SUSPENSO;
- BLOQUEADO-SUSPENSO;

As funcionalidades implementadas na api são:

- [x] ccreate;
- [x] cyield;
- [ ] cjoin;
- [ ] csuspend;
- [ ] cresume;
- [ ] csem_init;
- [ ] cwait;
- [ ] csignal;
- [x] cidentify;
