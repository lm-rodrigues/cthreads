#
# Makefile para programas exemplo
# 

FLAGS=-Wall -g
CC=gcc
LIB_DIR=../lib

all: exemplo mandel teste_vetor series

exemplo: exemplo.c $(LIB_DIR)/libcthread.a
	$(CC) $(FLAGS) -o exemplo exemplo.c -L$(LIB_DIR) -lcthread -Wall

teste_vetor: teste_vetor.c $(LIB_DIR)/libcthread.a
	$(CC) $(FLAGS) -o teste_vetor teste_vetor.c -L$(LIB_DIR) -lcthread -Wall

mandel: mandel.c $(LIB_DIR)/libcthread.a
	$(CC) $(FLAGS) -o mandel mandel.c -L$(LIB_DIR) -lcthread -lm -Wall

series: series.c $(LIB_DIR)/libcthread.a
	$(CC) $(FLAGS) -o series series.c -L$(LIB_DIR) -lcthread -lm -Wall
	
clean:
	rm -rf exemplo mandel teste_vetor barbeiro filosofos prodcons series *.o *~




